#ifndef INTERFAZ_H
#define INTERFAZ_H

#include <gtk/gtk.h>

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


void aplicar_filtros(GtkButton *button, gpointer user_data);
void activar_modo(GtkButton *button, gpointer user_data);
void suspender(GtkButton *button, gpointer user_data);
void eliminar(GtkButton *button, gpointer user_data);
void activate(GtkApplication *app, gpointer user_data);

#endif 