#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include "readproc.h"

typedef struct Node {
    proc_t *data;
    struct Node *next;
} Node;

Node *crearNodo(proc_t *proceso) {
    Node *newNode = malloc(sizeof(Node));
    newNode->data = malloc(sizeof(proc_t));
    memcpy(newNode->data, proceso, sizeof(proc_t));
    newNode->next = NULL;
    return newNode;
}

void imprimirLista(Node *head) {
    Node *actual = head;
    printf("PID\tCPU%%\tUSER\tCOMMAND\n");
    printf("-------------------------------------\n");
    while (actual != NULL) {
        printf("%d\t%.2f\t%s\t%s\n", actual->data->tid, (float)actual->data->pcpu, actual->data->ruser, actual->data->cmd);
        actual = actual->next;
    }
}

void freeLista(Node *head) {
    Node *actual = head;
    while (actual != NULL) {
        Node *next = actual->next;
        free(actual->data);
        free(actual);
        actual = next;
    }
}

Node *filtrar_por_cpu(Node **parametroHead, float limite_cpu, int primer_filtro) {
    if (primer_filtro == 1) {
        PROCTAB *proc = openproc(PROC_FILLSTAT | PROC_FILLSTATUS | PROC_FILLUSR);
        proc_t *proceso;
        Node *head = NULL;
        Node *nodoAnterior = NULL;

        while ((proceso = readproc(proc, NULL)) != NULL) {
            if (proceso->pcpu > limite_cpu) {
                Node *nuevoNodo = crearNodo(proceso);
                if (nodoAnterior == NULL) {
                    head = nuevoNodo;
                    nodoAnterior = nuevoNodo;
                } else {
                    nodoAnterior->next = nuevoNodo;
                    nodoAnterior = nuevoNodo;
                }
            }
            freeproc(proceso);
        }

        closeproc(proc);
        return head;
    }

    if (primer_filtro == 0 && parametroHead != NULL) {
        Node *nodeActual = *parametroHead;
        Node *nodeAnterior = NULL;

        while (nodeActual != NULL) {
            if (nodeActual->data->pcpu > limite_cpu) {
                if (nodeAnterior == NULL) {
                    *parametroHead = nodeActual->next;
                } else {
                    nodeAnterior->next = nodeActual->next;
                }
                Node *tmp = nodeActual;
                nodeActual = nodeActual->next;
                free(tmp->data);
                free(tmp);
                continue;
            }
            nodeAnterior = nodeActual;
            nodeActual = nodeActual->next;
        }
    }

    return NULL;
}

//Optimiza el proceso dado
void optimizar_proceso(int pid) {
    if (kill(pid, SIGKILL) == 0) {
        printf(" Proceso %d terminado exitosamente.\n", pid);
    } else {
        perror("❌ Error al terminar el proceso");
    }
}

int main() {
    Node *head = NULL;

    printf("Procesos con uso de CPU mayor al 1%%:\n\n");
    head = filtrar_por_cpu(NULL, 1.0, 1);
    imprimirLista(head);

    int opcion;
    printf("\n¿Deseas optimizar (terminar) un proceso? [1 = sí / 0 = no]: ");
    scanf("%d", &opcion);

    if (opcion == 1) {
        int pid;
        printf("Introduce el PID del proceso que deseas optimizar: ");
        scanf("%d", &pid);
        optimizar_proceso(pid);
    }

    freeLista(head);
    return 0;
}

