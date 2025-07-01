#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include "../proc/readproc.h"
#include "../include/Filtros.h"
#include "../include/control.h"



float calcular_porcentaje_cpu(int pid) {
    long clock_ticks = sysconf(_SC_CLK_TCK); // para obtener el número de ticks por segundo
    proc_t *proceso;
    proc_t p1 = {0}, p2 = {0}; // se usa sin ser punteros para evitar asiganar memoria y liberarla
    double uptime1, uptime2;
    FILE *uptime_file;
    PROCTAB *tab;

    // se obtienen los primeros datos
    uptime_file = fopen("/proc/uptime", "r"); // se abre en modo lectura 
    if (uptime_file == NULL) {
        return -1; // si no se puede abrir el archivo, se retorna -1
    }
    fscanf(uptime_file, "%lf", &uptime1); // fscanf lee el primer valor de uptime de tipo double
    fclose(uptime_file);
    tab = openproc(PROC_FILLSTAT); 
    proceso = readproc(tab, NULL);
    while (proceso != NULL) {
        if (proceso->tid == pid) { 
            memcpy(&p1, proceso, sizeof(proc_t)); // se copia todo el contenido
            break;
        }
        freeproc(proceso);
        proceso = readproc(tab, NULL);
    }
    closeproc(tab);

    double usage1_seconds = (p1.utime + p1.stime) / clock_ticks; // se calcula el uso de cpu en segundos

    sleep(1); // se espera un segundo para obtener otros datos en este intervalo

    // se obtienen los segundos datos
    uptime_file = fopen("/proc/uptime", "r");
    if (uptime_file == NULL) {
        return -1; 
    }
    fscanf(uptime_file, "%lf", &uptime2);
    fclose(uptime_file);

    tab = openproc(PROC_FILLSTAT);
    proceso = readproc(tab, NULL);
    while (proceso != NULL) {
        if (proceso->tid == pid) {
            memcpy(&p2, proceso, sizeof(proc_t));
            break;
        }
        freeproc(proceso);
        proceso = readproc(tab, NULL);
    }
    closeproc(tab);

    double usage2_seconds = (p2.utime + p2.stime) / clock_ticks;

    double diferencia_usage = usage2_seconds - usage1_seconds;
    double diferencia_uptime = uptime2 - uptime1;

    if (diferencia_uptime <= 0.0) {
        return 0.0; // esto quiere decir que el proceso no ha estado activo durante el intervalo de tiempo
    }
    float porcentaje = (diferencia_usage / diferencia_uptime) * 100.0;
    return porcentaje;
}

Node *filtrar_por_cpu(Node **parametroHead, float limite_cpu, int primer_filtro) {
    if (primer_filtro == 1) {
        PROCTAB *proc = openproc(PROC_FILLSTAT | PROC_FILLSTATUS);
        proc_t *proceso;
        Node *retornoHead = NULL;
        Node *nodoAnterior = NULL;
        int procesos_encontrados = 0;
        while ((proceso = readproc(proc, NULL)) != NULL) {
            if (calcular_porcentaje_cpu(proceso->tid) >= limite_cpu) { // se usa ">=" porque se quiere crear la lista apartir de los procesos que superan el limite
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
        return retornoHead;
    }

    if (primer_filtro == 0 && parametroHead != NULL) {
        Node *nodeActual = *parametroHead;
        Node *nodeAnterior = NULL;
        while (nodeActual != NULL) {
            if (calcular_porcentaje_cpu(nodeActual->data->tid) <= limite_cpu) { // se usa "<=" porque se quiere eliminar los procesos que no superan el limite, para que queden los que si
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

Node *filtrar_por_ram(Node **parametroHead, unsigned long limite_ram, int primer_filtro) {
    if (primer_filtro == 1) {
        PROCTAB *proc = openproc(PROC_FILLMEM | PROC_FILLSTAT | PROC_FILLSTATUS);
        proc_t *proceso;
        Node *retornoHead = NULL;
        Node *nodoAnterior = NULL;
        int procesos_encontrados = 0;
        while ((proceso = readproc(proc, NULL)) != NULL) {
            if (proceso->vm_rss >= limite_ram) {
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
        return retornoHead;
    }

    if (primer_filtro == 0 && parametroHead != NULL) {
        Node *nodeActual = *parametroHead;
        Node *nodeAnterior = NULL;
        while (nodeActual != NULL) {
            if (nodeActual->data->vm_rss <= limite_ram) {
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


void eliminar_procesos(Node **head) {
    Node *nodeActual = *head;
    while (nodeActual != NULL) {
        if (kill(nodeActual->data->tid, SIGKILL) == 0) {
            printf(" El proceso '%s' eliminado exitosamente. \n", nodeActual->data->cmd);
        } else {
            printf("❌ Error al terminar el proceso '%s'. \n", nodeActual->data->cmd);
        }
        nodeActual = nodeActual->next;
    }
    printf("\n");
}

void suspender_procesos(Node **head) {
    Node *nodeActual = *head;
    while (nodeActual != NULL) {
        if (kill(nodeActual->data->tid, SIGSTOP) == 0) {
            printf(" El proceso '%s' suspendido exitosamente. \n", nodeActual->data->cmd);

        } else {
            printf("❌ Error al suspender el proceso '%s'. \n", nodeActual->data->cmd);
        }
        nodeActual = nodeActual->next;
    }
    printf("\n");
}

void reanudar_procesos(Node **head) {
    Node *nodeActual = *head;
    while (nodeActual != NULL) {
        if (kill(nodeActual->data->tid, SIGCONT) == 0) {
            printf(" El proceso '%s' reanudado exitosamente. \n", nodeActual->data->cmd);
        } else {
            printf("❌ Error al reanudar el proceso '%s'. \n", nodeActual->data->cmd);
        }
        nodeActual = nodeActual->next;
    }
    printf("\n");
}
