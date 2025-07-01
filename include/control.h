#ifndef CONTROL_H
#define CONTROL_H

#include "../proc/readproc.h"

float calcular_porcentaje_cpu(int pid);
Node *filtrar_por_cpu(Node **parametroHead, float limite_cpu, int primer_filtro);
Node *filtrar_por_ram(Node **parametroHead, unsigned long limite_ram, int primer_filtro);
void eliminar_procesos(Node **head);
void suspender_procesos(Node **head);
void reanudar_procesos(Node **head);


#endif