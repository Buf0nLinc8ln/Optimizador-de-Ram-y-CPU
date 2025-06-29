#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../proc/readproc.h"
#include "../include/Filtros.h"

// Optimizador de memoria RAM

Node *crearNodo(proc_t *proceso){

    Node *newNode = malloc(sizeof(Node));

    newNode->data = malloc(sizeof(proc_t)); // se reserva memoria dinamica para la data de tipo proc_t del nodo
    
    memcpy(newNode->data, proceso, sizeof(proc_t)); // se usa memcpy para copiar el contenido del proceso tipo proc_t a los datos del nuevo nodo 
 
    newNode->next = NULL;
    return newNode;
}


void imprimirLista(Node *head) {
    
    struct Node *nodeActual = head;
    while(nodeActual != NULL) {
        printf("PID: %d | PPID: %d | GPID: %d | USER: (%d)%s | GROUP: (%d)%s | ESTADO: %c | TTY: %d | TPGID: %d | NAME: %s \n",
               nodeActual->data->tid, 
               nodeActual->data->ppid, 
               nodeActual->data->pgrp, 
               nodeActual->data->ruid, 
               nodeActual->data->ruser, 
               nodeActual->data->rgid, 
               nodeActual->data->rgroup, 
               nodeActual->data->state,
               nodeActual->data->tty,
               nodeActual->data->tpgid,
               nodeActual->data->cmd);
        nodeActual = nodeActual->next;

    }

}



void EliminarProcesoLista(struct Node **head, int pid) {
    struct Node *nodeActual = *head;
    struct Node *nodeAnterior = NULL;
    while (nodeActual != NULL) {
        if (nodeActual->data->tid == pid) {
            if (nodeAnterior == NULL) {
                // en el caso que "nodeAnterior" sea NULL, significa que el nodo a eliminar es la cabeza de la lista, tons se actualiza la cabeza
                *head = nodeActual->next; 
            } else {
                nodeAnterior->next = nodeActual->next;
            }
            free(nodeActual->data);
            free(nodeActual);
            return; // se sale de la funcion cuando  se elimina el nodo
        }
        nodeAnterior = nodeActual;
        nodeActual = nodeActual->next;
    }
}

void freeLista(struct Node *head) {
    struct Node *nodeActual = head;
    while (nodeActual != NULL) {
        struct Node *nodoSiguiente = nodeActual->next;
        free(nodeActual->data); 
        free(nodeActual);
        nodeActual = nodoSiguiente;
    }
}

int filtrar_por_pid(int pid) {

    // la funcion recibe como parametro el pid del proceso
    // PROC_FILLSTAT: lee stat, que contiene el estado del proceso y en este caso la segunda columna es el nombre del proceso

    PROCTAB *proc = openproc(PROC_FILLSTAT); 

    proc_t *proceso; // puntero de tipo proc_t, estructura que contiene la informacion de un proceso
    while ((proceso = readproc(proc, NULL)) != NULL) {

        if (proceso->tid == pid) { // tid contiene el pid del proceso

            Node *nuevoNodo = crearNodo(proceso); // crea un nuevo nodo con el proceso actual (en este caso va ser el unico nodo de la lista)

        }

        freeproc(proceso); // libera la memoria del proceso actual
    }

    closeproc(proc); // cierra el acceso a /proc

    return 1; // se retorna 1 si se uso esta funcion para filtrar por pid
}


Node *filtrar_por_uid(Node **parametroHead, int uid, char *user, int primer_filtro) { 
    // la funcion recibe como parametro el UID del proceso, una cadena de caracteres con el nombre de usuario , y los predeterminodos para todas las funciones de filtrado (Node **parametroHead, int primer_filtro)
    if (primer_filtro == 1) {    //  // si primer_filtro es 1, significa que se quiere generar una lista enlazada con los procesos filtrados por UID o nombre de usuario
        // PROC_FILLSTAT: lee stat, que contiene el estado del proceso
        // PROC_FILLSTATUS: lee status, que contiene informacion del proceso, y en este caso la nuvena fila es el UID del proceso
        // PROC_FILLUSR: resolve user id number -> user name (resuelve el numero de id de usuario a nombre de usuario)
        // PROC_FILLGRP: resolve group id number -> group name (resuelve el numero de id de grupo a nombre de grupo)

        PROCTAB *proc = openproc(PROC_FILLSTAT | PROC_FILLSTATUS | PROC_FILLUSR | PROC_FILLGRP); 

        proc_t *proceso;
        Node *retornoHead = NULL; // se guarda la cabeza de la lista enlazada, ya que es el primer filtro que se aplica
        Node *nodoAnterior = NULL; // puntero para el nodo anterior
        int procesos_encontrados = 0; // contador de procesos encontrados
        while ((proceso = readproc(proc, NULL)) != NULL)  {
            if ((uid == -1 && strcmp(proceso->ruser, user) == 0) || (user == NULL && proceso->ruid == uid)) { // si la funcion recibe -1 como aparametro para uid, se filtran los procesos con el nombre de usuario.
                // ruser contiene el nombre de usuario real que ejecuta el proceso. Se usa strcmp para comparar cadenas de caracteres
                Node *nuevoNodo = crearNodo(proceso);
                if (nodoAnterior == NULL) {
                    // Si es el primer nodo, lo asignamos a nodoAnterior y a la cabeza de la lista
                    retornoHead = nuevoNodo; 
                    nodoAnterior = nuevoNodo; 
                } else {
                    nodoAnterior->next = nuevoNodo; // se enlaza el nodo anterior con el nuevo nodo
                    nodoAnterior = nuevoNodo; // se actualiza el nodo anterior con el nuevo nodo
                }
                procesos_encontrados++; // se incrementa en 1 el contador de procesos encontrados
            }
            freeproc(proceso);
        }
        closeproc(proc);
        if (procesos_encontrados == 0) {
            return NULL; // se retorna NULL si no se encontraron procesos
        }
        return retornoHead; // se retorna la cabeza de la lista enlazada con los procesos filtrados por UID o nombre de usuario
    }

    if (primer_filtro == 0) { // si primer_filtro es 0, significa que se quiere eliminar procesos de la lista enlazada
        
        struct Node *nodeActual = *parametroHead;
        struct Node *nodeAnterior = NULL;
        while (nodeActual != NULL) {
            if ((uid == -1 && strcmp(nodeActual->data->ruser, user) == 0) || (user == NULL && nodeActual->data->ruid == uid)) { 
                if (nodeAnterior == NULL) {
                    // en el caso que "nodeAnterior" sea NULL, significa que el nodo a eliminar es la cabeza de la lista, tons se actualiza la cabeza
                    *parametroHead = nodeActual->next; 
                } else {
                    nodeAnterior->next = nodeActual->next;
                }
                Node *nodeTemporal = nodeActual; // se crea un nodo temporal para liberar la memoria del nodo actual
                nodeActual = nodeActual->next;  // se actualiza el nodo actual al siguiente nodo
                free(nodeTemporal->data);
                free(nodeTemporal);
                continue;
            }
            nodeAnterior = nodeActual;
            nodeActual = nodeActual->next;
        }
    }
}

Node *filtrar_por_gid(Node **parametroHead, int gid, char *group, int primer_filtro) { 
    // la funcion recibe como parametro el GID del proceso y una cadena de caracteres con el nombre de grupo

    if (primer_filtro == 1) { 

        PROCTAB *proc = openproc(PROC_FILLSTAT | PROC_FILLSTATUS | PROC_FILLUSR | PROC_FILLGRP); 

        proc_t *proceso;
        Node *retornoHead = NULL; 
        Node *nodoAnterior = NULL;
        int procesos_encontrados = 0; 
        while ((proceso = readproc(proc, NULL)) != NULL)  {

            if ((gid == -1 && strcmp(proceso->rgroup, group) == 0) || (group == NULL && proceso->ruid == gid)) { // si la funcion recibe -1 como aparametro para gid, se filtran los procesos con el nombre de grupo.
                // rgroup contiene el nombre de grupo real que ejecuta el proceso. 
                Node *nuevoNodo = crearNodo(proceso);
                if (nodoAnterior == NULL) {
                    retornoHead = nuevoNodo; 
                    nodoAnterior = nuevoNodo; 
                } else {
                    nodoAnterior->next = nuevoNodo; 
                    nodoAnterior = nuevoNodo; 
                }
                procesos_encontrados++; 
            }
            freeproc(proceso);
        }
        closeproc(proc);
        if (procesos_encontrados == 0) {
            return NULL;
        }
        return retornoHead; // se retorna la cabeza de la lista enlazada con los procesos filtrados por GID o nombre de grupo
    }

    if (primer_filtro == 0) { // si primer_filtro es 0, significa que se quiere eliminar procesos de la lista enlazada
        
        struct Node *nodeActual = *parametroHead;
        struct Node *nodeAnterior = NULL;
        while (nodeActual != NULL) {
            if ((gid == -1 && strcmp(nodeActual->data->ruser, group) == 0) || (group == NULL && nodeActual->data->ruid == gid)) {
                if (nodeAnterior == NULL) {
                    
                    *parametroHead = nodeActual->next; 
                } else {
                    nodeAnterior->next = nodeActual->next;
                }
                Node *nodeTemporal = nodeActual; 
                nodeActual = nodeActual->next;  
                free(nodeTemporal->data);
                free(nodeTemporal);
                continue;
            }
            nodeAnterior = nodeActual;
            nodeActual = nodeActual->next;
        }
    }
}


Node *filtrar_por_nombre(Node **parametroHead, char *nombre, int primer_filtro) {
    // la funcion recibe como parametro el nombre del proceso y la orden de generar o no la lista enlazada con los procesos filtrados por nombre

    if (primer_filtro == 1) {
             
        PROCTAB *proc = openproc(PROC_FILLSTAT | PROC_FILLSTATUS | PROC_FILLUSR | PROC_FILLGRP); 

        proc_t *proceso;
        Node *retornoHead = NULL; 
        Node *nodoAnterior = NULL;
        int procesos_encontrados = 0;
        while ((proceso = readproc(proc, NULL)) != NULL) {
            if (strcmp(proceso->cmd, nombre) == 0) { // cmd contiene el nombre del proceso
                Node *nuevoNodo = crearNodo(proceso);
                if (nodoAnterior == NULL) {
                    retornoHead = nuevoNodo; 
                    nodoAnterior = nuevoNodo; 
                } else {
                    nodoAnterior->next = nuevoNodo; 
                    nodoAnterior = nuevoNodo; // se actualiza el nodo anterior con el nuevo nodo
                }
                procesos_encontrados++; 
            }
            freeproc(proceso); 
        }
        closeproc(proc);
        if (procesos_encontrados == 0) {
            return NULL; 
        }
        return retornoHead; 
    }
    
    if (primer_filtro == 0) { // si primer_filtro es 0, significa que se quiere eliminar un proceso de la lista enlazada
        
        struct Node *nodeActual = *parametroHead;
        struct Node *nodeAnterior = NULL;
        while (nodeActual != NULL) {
            if (strcmp(nodeActual->data->cmd, nombre) == 0) {
                if (nodeAnterior == NULL) {
                    // en el caso que "nodeAnterior" sea NULL, significa que el nodo a eliminar es la cabeza de la lista, tons se actualiza la cabeza
                    *parametroHead = nodeActual->next; 
                } else {
                    nodeAnterior->next = nodeActual->next;
                }
                Node *nodeTemporal = nodeActual;
                nodeActual = nodeActual->next; 
                free(nodeTemporal->data);
                free(nodeTemporal);
                continue;
            }
            nodeAnterior = nodeActual;
            nodeActual = nodeActual->next;
        }
    }
}

Node *filtrar_por_ppid(Node **parametroHead, int ppid, int primer_filtro) {
    // la funcion recibe como parametro el PPID del proceso

   if (primer_filtro == 1) { // si primer_filtro es 1, significa que se quiere generar una lista enlazada con los procesos filtrados por PPID
        PROCTAB *proc = openproc(PROC_FILLSTAT | PROC_FILLSTATUS | PROC_FILLUSR | PROC_FILLGRP); 
        proc_t *proceso;
        Node *retornoHead = NULL; 
        Node *nodoAnterior = NULL;
        int procesos_encontrados = 0; 
        while ((proceso = readproc(proc, NULL)) != NULL)  {
            if (proceso->ppid == ppid) { // ppid contiene el PPID del proceso
                Node *nuevoNodo = crearNodo(proceso);
                if (nodoAnterior == NULL) {
                    retornoHead = nuevoNodo; 
                    nodoAnterior = nuevoNodo; 
                } else {
                    nodoAnterior->next = nuevoNodo; 
                    nodoAnterior = nuevoNodo; 
                }
                procesos_encontrados++; 
            }
            freeproc(proceso);
        }
        closeproc(proc);
        if (procesos_encontrados == 0) {
            return NULL; 
        }
        return retornoHead; // se retorna la cabeza de la lista enlazada con los procesos filtrados por PPID
    }

    if (primer_filtro == 0) { 
        
        struct Node *nodeActual = *parametroHead;
        struct Node *nodeAnterior = NULL;
        while (nodeActual != NULL) {
            if (nodeActual->data->ppid == ppid) {
                if (nodeAnterior == NULL) {
                    *parametroHead = nodeActual->next; 
                } else {
                    nodeAnterior->next = nodeActual->next;
                }
                Node *nodeTemporal = nodeActual;
                nodeActual = nodeActual->next; 
                free(nodeTemporal->data);
                free(nodeTemporal);
                continue;
            }
            nodeAnterior = nodeActual;
            nodeActual = nodeActual->next;
        }
    }
}

Node *filtrar_por_gpid(Node **parametroHead, int gpid, int primer_filtro) {
    // la funcion recibe como parametro el GPID del proceso
    if (primer_filtro == 1) { // si primer_filtro es 1, significa que se quiere generar una lista enlazada con los procesos filtrados por GPID
        PROCTAB *proc = openproc(PROC_FILLSTAT | PROC_FILLSTATUS | PROC_FILLUSR | PROC_FILLGRP); 
        proc_t *proceso;
        Node *retornoHead = NULL; 
        Node *nodoAnterior = NULL;
        int procesos_encontrados = 0;
        while ((proceso = readproc(proc, NULL)) != NULL)  {
            if (proceso->pgrp == gpid) { // pgrp contiene el GPID del proceso
                Node *nuevoNodo = crearNodo(proceso);
                if (nodoAnterior == NULL) {
                    retornoHead = nuevoNodo; 
                    nodoAnterior = nuevoNodo; 
                } else {
                    nodoAnterior->next = nuevoNodo; 
                    nodoAnterior = nuevoNodo; 
                }
                procesos_encontrados++;
            }
            freeproc(proceso);
        }
        closeproc(proc);
        if (procesos_encontrados == 0) {
            return NULL; 
        }
        return retornoHead; // se retorna la cabeza de la lista enlazada con los procesos filtrados por GPID
    }

    if (primer_filtro == 0) { 
        struct Node *nodeActual = *parametroHead;
        struct Node *nodeAnterior = NULL;
        while (nodeActual != NULL) {
            if (nodeActual->data->pgrp == gpid) {
                if (nodeAnterior == NULL) {
                    *parametroHead = nodeActual->next; 
                } else {
                    nodeAnterior->next = nodeActual->next;
                }
                Node *nodeTemporal = nodeActual;
                nodeActual = nodeActual->next; 
                free(nodeTemporal->data);
                free(nodeTemporal);
                continue;
            }
            nodeAnterior = nodeActual;
            nodeActual = nodeActual->next;
        }
    }
}

Node *filtrar_por_estado(Node **parametroHead, char estado, int primer_filtro) {
    // la funcion recibe como parametro el estado (S, R, Z, T, D) del proceso
    if (primer_filtro == 1) { // si primer_filtro es 1, significa que se quiere generar una lista enlazada con los procesos filtrados por estado
        PROCTAB *proc = openproc(PROC_FILLSTAT | PROC_FILLSTATUS | PROC_FILLUSR | PROC_FILLGRP); 
        proc_t *proceso;
        Node *retornoHead = NULL; 
        Node *nodoAnterior = NULL;
        int procesos_encontrados = 0; 
        while ((proceso = readproc(proc, NULL)) != NULL)  {
            if (proceso->state == estado || (proceso->tty != 0 && ((estado == 'F' && proceso->pgrp == proceso->tpgid) || (estado == 'B' && proceso->pgrp != proceso->tpgid))) ) { // state contiene el estado del proceso, y se filtran los procesos que estan en estado de foreground(primer plano) o background(segundo plano)
                // si el estado es 'F', se filtran los procesos que estan en primer plano
                // si el estado es 'B', se filtran los procesos que estan en segundo plano
                // tty contiene el terminal asociado al proceso, y tpgid contiene el grupo de procesos
                // si tty es diferente de 0, significa que el proceso esta asociado a un terminal, y ademas se verifica si el proceso esta en primer plano o segundo plano comparando el pgrp y tpgid
                Node *nuevoNodo = crearNodo(proceso);
                if (nodoAnterior == NULL) {
                    retornoHead = nuevoNodo; 
                    nodoAnterior = nuevoNodo; 
                } else {
                    nodoAnterior->next = nuevoNodo; 
                    nodoAnterior = nuevoNodo; 
                }
                procesos_encontrados++; 
            }
            freeproc(proceso);
        }
        closeproc(proc);
        if (procesos_encontrados == 0) {
            return NULL; 
        }
        return retornoHead; // se retorna la cabeza de la lista enlazada con los procesos filtrados por estado
    }

    if (primer_filtro == 0) { 
        struct Node *nodeActual = *parametroHead;
        struct Node *nodeAnterior = NULL;
        while (nodeActual != NULL) {
            if (nodeActual->data->state == estado) {
                if (nodeAnterior == NULL) {
                    *parametroHead = nodeActual->next; 
                } else {
                    nodeAnterior->next = nodeActual->next;
                }
                Node *nodeTemporal = nodeActual;
                nodeActual = nodeActual->next; 
                free(nodeTemporal->data);
                free(nodeTemporal);
                continue;
            }
            nodeAnterior = nodeActual;
            nodeActual = nodeActual->next;
        }
    }
}