#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../proc/readproc.h"
#include </usr/include/gtk-4.0/gtk/gtk.h>
#include "../Optimizador-de-Ram-y-CPU/include/Filtros.h"
#include "../Optimizador-de-Ram-y-CPU/include/interfaz.h"



int main(int argc, char **argv) {
    GtkApplication *app = gtk_application_new("org.ejemplo.filtro_procesos", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);


    freeLista(filtros.lista);

    return status;
}