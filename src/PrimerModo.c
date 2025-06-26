#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../proc/readproc.h"

// Optimizador de memoria RAM

typedef struct Node {
    proc_t *data;
    struct Node *next;
} Node;


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
        printf("PID: %d | NAME: %s | USER_NAME: %s | USER_ID: %d \n", nodeActual->data->tid, nodeActual->data->cmd, nodeActual->data->ruser, nodeActual->data->ruid);
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

int filtrar_por_pid(pid_t pid) {

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
        Node *head = NULL; // se guarda la cabeza de la lista enlazada, ya que es el primer filtro que se aplica
        Node *nodoAnterior = NULL; // puntero para el nodo anterior
        while ((proceso = readproc(proc, NULL)) != NULL)  {

            if (uid == -1) { // si la funcion recibe -1 como aparametro para uid, se filtran los procesos con el nombre de usuario.

                if (strcmp(proceso->ruser, user) == 0) { // ruser contiene el nombre de usuario real que ejecuta el proceso. Se usa strcmp para comparar cadenas de caracteres
                    Node *nuevoNodo = crearNodo(proceso);
                    if (nodoAnterior == NULL) {
                        // Si es el primer nodo, lo asignamos a nodoAnterior y a la cabeza de la lista
                        head = nuevoNodo; 
                        nodoAnterior = nuevoNodo; 
                    } else {
                        nodoAnterior->next = nuevoNodo; // se enlaza el nodo anterior con el nuevo nodo
                        nodoAnterior = nuevoNodo; // se actualiza el nodo anterior con el nuevo nodo
                    }
                    freeproc(proceso); 
                }

            }

            if (user == NULL) { // si la funcion recibe NULL como aparametro para user, se filtran los procesos con el UID.

                if (proceso->ruid == uid) { // ruid contiene el UID real del proceso
                    Node *nuevoNodo = crearNodo(proceso);
                    if (nodoAnterior == NULL) { 
                        head = nuevoNodo; 
                        nodoAnterior = nuevoNodo;
                    } else {
                        nodoAnterior->next = nuevoNodo; 
                        nodoAnterior = nuevoNodo; 
                    }
                    freeproc(proceso); 
                }

            }

        }
        closeproc(proc);
        return head; // se retorna la cabeza de la lista enlazada con los procesos filtrados por UID o nombre de usuario
    }

    if (primer_filtro == 0) { // si primer_filtro es 0, significa que se quiere eliminar procesos de la lista enlazada
        
        struct Node *nodeActual = *parametroHead;
        struct Node *nodeAnterior = NULL;
        while (nodeActual != NULL) {
            if (uid == -1) { 
                if (strcmp(nodeActual->data->ruser, user) == 0) {
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
            }
            if (user == NULL) { 
                if (nodeActual->data->ruid == uid) {
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
            }
            nodeAnterior = nodeActual;
            nodeActual = nodeActual->next;
        }
    }
    
}

Node *filtrar_por_gid(Node *parametroHead, int gid, char *group, int primer_filtro) { 
    // la funcion recibe como parametro el GID del proceso y una cadena de caracteres con el nombre de grupo

    PROCTAB *proc = openproc(PROC_FILLSTAT | PROC_FILLSTATUS | PROC_FILLUSR | PROC_FILLGRP); 

    proc_t *proceso;
    Node *head = NULL; 
    Node *nodoAnterior = NULL; 
    while ((proceso = readproc(proc, NULL)) != NULL)  {

        if (gid == -1) { // si la funcion recibe -1 como aparametro para gid, se filtran los procesos con el nombre de grupo.

            if (strcmp(proceso->rgroup, group) == 0) { // rgroup contiene el nombre de grupo real que ejecuta el proceso. Se usa strcmp para comparar cadenas de caracteres
                Node *nuevoNodo = crearNodo(proceso);
                if (nodoAnterior == NULL) {
                    head = nuevoNodo; 
                    nodoAnterior = nuevoNodo; 
                } else {
                    nodoAnterior->next = nuevoNodo; 
                    nodoAnterior = nuevoNodo; 
                }
                freeproc(proceso); 
            }

        }

        if (group == NULL) { // si la funcion recibe NULL como aparametro para group, se filtran los procesos con el GID.

            if (proceso->rgid == gid) { // rgid contiene el GID real del proceso
                Node *nuevoNodo = crearNodo(proceso);
                if (nodoAnterior == NULL) { 
                    head = nuevoNodo; 
                    nodoAnterior = nuevoNodo;
                } else {
                    nodoAnterior->next = nuevoNodo; 
                    nodoAnterior = nuevoNodo; 
                }
                freeproc(proceso); 
            }

        }

    }
    
    closeproc(proc);
    return head; // se retorna la cabeza de la lista enlazada con los procesos filtrados por GID o nombre de grupo
}



Node *filtrar_por_nombre(struct Node **parametroHead, const char *nombre, int primer_filtro) {
    // la funcion recibe como parametro el nombre del proceso y la orden de generar o no la lista enlazada con los procesos filtrados por nombre

    if (primer_filtro == 1) {
             
        PROCTAB *proc = openproc(PROC_FILLSTAT | PROC_FILLSTATUS | PROC_FILLUSR | PROC_FILLGRP); 

        proc_t *proceso;
        Node *retornoHead = NULL; 
        Node *nodoAnterior = NULL; 
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
            }
            freeproc(proceso); 
        }

        closeproc(proc);
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

int main() {
    

    Node *head = NULL;
    head = filtrar_por_uid(NULL, 1003, NULL, 1);
    imprimirLista(head);
    printf("\n\n");
    filtrar_por_nombre(&head, "brave", 0); 
    imprimirLista(head);

    freeLista(head); 

    printf("\n\n");
    head = filtrar_por_nombre(NULL, "brave", 1);

    imprimirLista(head);



    return 0;
}
