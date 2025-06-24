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

    newNode->data = malloc(sizeof(proc_t));
    
    memcpy(newNode->data, proceso, sizeof(proc_t)); // para copiar el contenido del proceso tipo proc_t a los datos del nuevo nodo 
 
    newNode->next = NULL;
    return newNode;
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





void filtrar_por_nombre(const char *nombre) {
    // la funcion recibe como parametro el nombre del proceso
             
    PROCTAB *proc = openproc(PROC_FILLSTAT); 

    proc_t *proceso; 
    while ((proceso = readproc(proc, NULL)) != NULL) {
        if (proceso->cmd && strcmp(proceso->cmd, nombre) == 0) { // cmd contiene el nombre del proceso

            printf("PID: %d, Nombre: %s\n", proceso->tid, proceso->cmd);
        }
        freeproc(proceso); 
    }

    closeproc(proc); 

}

int main() {
    
    
    char *nombre = "brave";


    filtrar_por_nombre(nombre);

    return 0;
}
