#ifndef INTERFAZ_H
#define INTERFAZ_H

#include <gtk/gtk.h>
#include "../include/Filtros.h"




// estructura que contiene los filtros ha aplicar a los procesos
typedef struct {
    int pid, uid, gid, ppid, gpid;
    char *user;
    char *group;
    char *nombre;
    char estado;
    long limite_ram;
    float limite_cpu;
} Filtros;

// estructura para almacenar variables globales
typedef struct {
    Filtros filtros;
    Node *lista;
    char *modo;
    int primer_filtro; // para indicar si es o no el primer filtro aplicado
    int suspendido; // para ver si se uso la funcion suspender
    int filtrado_por_pid;
} Global;

// estructura para liberar, inicializar y eliminar datos, al cerrar ventana
typedef struct {
    GtkWindow *window;
    Global *global;
} Salida;

extern GtkWidget *entry_pid;
extern GtkWidget *entry_uid;
extern GtkWidget *entry_gid;
extern GtkWidget *entry_nombre;
extern GtkWidget *entry_ppid;
extern GtkWidget *entry_gpid;
extern GtkWidget *entry_estado;
extern GtkWidget *entry_ram;
extern GtkWidget *estado_buttons[5];
extern gint estado_seleccionado;
extern GtkWidget *entry_ram_limit;
extern GtkWidget *entry_cpu_limit;
extern GtkWidget *button_suspend;
extern GtkWidget *button_eliminate;

int es_entero(char *cadena);
void aplicar_filtros(GtkButton *button, gpointer user_data);
void activar_modo(GtkButton *button, gpointer user_data);
void suspender(GtkButton *button, gpointer user_data);
void eliminar(GtkButton *button, gpointer user_data);
void reanudar(GtkWindow *window, Node **lista);
void activate(GtkApplication *app, gpointer user_data);

#endif 