#ifndef CONTROL_H
#define CONTROL_H

#include "../proc/readproc.h"

float calcular_porcentaje_cpu(pid_t pid);
Node *filtrar_por_cpu(Node **parametroHead, float limite_cpu, int primer_filtro);
Node *filtrar_por_ram(Node **parametroHead, long limite_ram, int primer_filtro);
int eliminar_procesos(Node *head);
int suspender_procesos(Node *head);
int reanudar_procesos(Node *head);


#endif