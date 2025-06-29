#ifndef FILTROS_H
#define FILTROS_H

#include "../proc/readproc.h"

typedef struct Node {
    proc_t *data;
    struct Node *next;
} Node;

Node *crearNodo(proc_t *proceso);
void imprimirLista(Node *head);
void EliminarProcesoLista(struct Node **head, int pid);
void freeLista(struct Node *head);

int filtrar_por_pid(int pid);
Node *filtrar_por_uid(Node **parametroHead, int uid, char *user, int primer_filtro);
Node *filtrar_por_gid(Node **parametroHead, int gid, char *group, int primer_filtro);
Node *filtrar_por_nombre(Node **parametroHead, const char *nombre, int primer_filtro);
Node *filtrar_por_ppid(Node **parametroHead, int ppid, int primer_filtro);
Node *filtrar_por_gpid(Node **parametroHead, int gpid, int primer_filtro);
Node *filtrar_por_estado(Node **parametroHead, char estado, int primer_filtro);


#endif