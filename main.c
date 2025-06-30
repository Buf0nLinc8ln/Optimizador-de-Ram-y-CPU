#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include "../proc/readproc.h"
#include "/home/lincoln/Optimizador-de-Ram-y-CPU/include/Filtros.h"
#include "/home/lincoln/Optimizador-de-Ram-y-CPU/include/interfaz.h"
#include "/home/lincoln/Optimizador-de-Ram-y-CPU/include/control.h"

int main(int argc, char **argv) {
    GtkApplication *app = gtk_application_new("org.ejemplo.filtro_procesos", G_APPLICATION_DEFAULT_FLAGS);
    Global *datos = g_new0(Global, 1);
    g_object_set_data(G_OBJECT(app), "datos_global", datos);
    g_signal_connect(app, "activate", G_CALLBACK(activate), datos);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    Global *datos_guardados = g_object_get_data(G_OBJECT(app), "datos_global");
    if (datos_guardados != NULL) {
        freeLista(datos_guardados->lista);
        g_free(datos_guardados);
    }
    g_object_unref(app);
    return status;
}