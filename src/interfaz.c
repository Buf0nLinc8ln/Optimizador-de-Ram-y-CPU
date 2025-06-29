#include <gtk/gtk.h>

GtkWidget *entry_pid, *entry_uid, *entry_gid, *entry_nombre, *entry_ppid, *entry_gpid, *entry_estado;
GtkWidget *estado_buttons[5];
gint estado_seleccionado = 0;
GtkWidget *entry_ram_limit;
GtkWidget *entry_cpu_limit;
GtkWidget *button_suspend;
GtkWidget *button_eliminate;


static void aplicar_filtros(GtkButton *button, gpointer user_data) {
   
    char *pid_text = g_strstrip(g_strdup(gtk_editable_get_text(GTK_EDITABLE(entry_pid))));
    char *uid_text = g_strstrip(g_strdup(gtk_editable_get_text(GTK_EDITABLE(entry_uid))));
    char *gid_text = g_strstrip(g_strdup(gtk_editable_get_text(GTK_EDITABLE(entry_gid))));
    char *nombre = g_strstrip(g_strdup(gtk_editable_get_text(GTK_EDITABLE(entry_nombre))));
    char *ppid_text = g_strstrip(g_strdup(gtk_editable_get_text(GTK_EDITABLE(entry_ppid))));
    char *gpid_text = g_strstrip(g_strdup(gtk_editable_get_text(GTK_EDITABLE(entry_gpid))));
    char *estado = g_strstrip(g_strdup(gtk_editable_get_text(GTK_EDITABLE(entry_estado))));

    int pid = atoi(pid_text); // "atoi" convierte texto a entero
    int uid = atoi(uid_text);
    int gid = atoi(gid_text);
    int ppid = atoi(ppid_text);
    int gpid = atoi(gpid_text);
}


static void activar_modo(GtkButton *button, gpointer user_data) {
    const gchar *modo = (const gchar *)user_data;
    

    gtk_widget_set_visible(entry_ram_limit, g_strcmp0(modo, "RAM") == 0);
    gtk_widget_set_visible(entry_cpu_limit, g_strcmp0(modo, "CPU") == 0);

    gtk_widget_set_visible(button_suspend, TRUE);
    gtk_widget_set_visible(button_eliminate, TRUE);
}


static void suspender(GtkButton *button, gpointer user_data) {
     g_print("Procesos suspendidos");
}


static void eliminar(GtkButton *button, gpointer user_data) {
     g_print("Procesos eliminados");
}


// static void on_reanudar_response(GObject *source_object, GAsyncResult *res, gpointer user_data) {
//     GtkAlertDialog *alert = GTK_ALERT_DIALOG(source_object);
//     int response = gtk_alert_dialog_choose_finish(alert, res, NULL);


//     if (response == 0) {
//     g_print("Reanudar procesos\n");
//     } else {
//     g_print("Los procesos no se reanudarán\n");
//     }
// }
// static void reanudar(GtkWindow *parent_window) {
//     const char *buttons[] = {"Sí", "No", NULL};
//     GtkAlertDialog *alert = gtk_alert_dialog_new("¿Reanudar procesos?");
//     gtk_alert_dialog_set_buttons(alert, buttons);

//     gtk_alert_dialog_choose(alert, parent_window, NULL, on_reanudar_response, NULL);
// }


// static void on_confirmar_salida_response(GObject *source_object, GAsyncResult *res, gpointer user_data) {
//     GtkAlertDialog *alert = GTK_ALERT_DIALOG(source_object);
//     int response = gtk_alert_dialog_choose_finish(alert, res, NULL);

//     if (response == 0) {
//         g_print("Reanudar procesos antes de salir\n");
//         reanudar(GTK_WINDOW(user_data));
//     } else {
//         g_print("Saliendo sin reanudar procesos\n");
//         gtk_window_destroy(GTK_WINDOW(user_data));
//     }

//     g_object_unref(alert);
// }

// static gboolean confirmar_salida(GtkWindow *window, gpointer user_data) {
//     const char *buttons[] = {"Sí", "No", NULL};
//     GtkAlertDialog *alert = gtk_alert_dialog_new("¿Reanudar procesos?");
//     gtk_alert_dialog_set_buttons(alert, buttons);

//     gtk_alert_dialog_choose(alert, window, NULL, on_confirmar_salida_response, NULL);

//     return TRUE;
// }

static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Filtro de Procesos");
    gtk_window_set_default_size(GTK_WINDOW(window), 500, 500);

    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 5);
    gtk_window_set_child(GTK_WINDOW(window), grid);

    int row = 0;
    #define ADD_ENTRY(label, widget) \
        gtk_grid_attach(GTK_GRID(grid), gtk_label_new(label), 0, row, 1, 1); \
        widget = gtk_entry_new(); \
        gtk_grid_attach(GTK_GRID(grid), widget, 1, row++, 1, 1);

    ADD_ENTRY("PID:", entry_pid);
    ADD_ENTRY("UID:", entry_uid);
    ADD_ENTRY("GID:", entry_gid);
    ADD_ENTRY("Nombre:", entry_nombre);
    ADD_ENTRY("PPID:", entry_ppid);
    ADD_ENTRY("GPID:", entry_gpid);
    // gtk_grid_attach(GTK_GRID(grid),
    // gtk_label_new(
    //     "Estados posibles: F (foreground), B (background), R (Running), S (Sleeping), Z (Zombie) T (Suspendido), D (Espera ininterrumpida)"
    // ),
    // 0, row++, 2, 1
    // );
    ADD_ENTRY("Estado:", entry_estado);
    // tooltip sirve para mostrar información adicional al pasar el cursor sobre el widget
    gtk_widget_set_tooltip_text(entry_estado, "Estados posibles: F (foreground), B (background), R (Running), S (Sleeping), Z (Zombie) T (Suspendido), D (Espera ininterrumpida)");

    
    GtkWidget *boton = gtk_button_new_with_label("Aplicar Filtros");
    gtk_grid_attach(GTK_GRID(grid), boton, 0, row++, 2, 1);
    g_signal_connect(boton, "clicked", G_CALLBACK(aplicar_filtros), NULL);

    GtkWidget *separator = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_grid_attach(GTK_GRID(grid), separator, 0, row++, 2, 1);

    GtkWidget *button_modo_ram = gtk_button_new_with_label("Modo 'RAM'");
    gtk_grid_attach(GTK_GRID(grid), button_modo_ram, 0, row, 1, 1);
    g_signal_connect(button_modo_ram, "clicked", G_CALLBACK(activar_modo), "RAM");

    GtkWidget *button_modo_cpu = gtk_button_new_with_label("Modo 'CPU'");
    gtk_grid_attach(GTK_GRID(grid), button_modo_cpu, 1, row++, 1, 1);
    g_signal_connect(button_modo_cpu, "clicked", G_CALLBACK(activar_modo), "CPU");

    entry_ram_limit = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_ram_limit), "Límite RAM");
    gtk_grid_attach(GTK_GRID(grid), entry_ram_limit, 0, row++, 2, 1);
    gtk_widget_set_visible(entry_ram_limit, FALSE);

    entry_cpu_limit = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_cpu_limit), "Límite CPU");
    gtk_grid_attach(GTK_GRID(grid), entry_cpu_limit, 0, row++, 2, 1);
    gtk_widget_set_visible(entry_cpu_limit, FALSE);

    button_suspend = gtk_button_new_with_label("Suspender");
    gtk_grid_attach(GTK_GRID(grid), button_suspend, 0, row, 1, 1);
    g_signal_connect(button_suspend, "clicked", G_CALLBACK(suspender), NULL);
    gtk_widget_set_visible(button_suspend, FALSE);

    button_eliminate = gtk_button_new_with_label("Eliminar");
    gtk_grid_attach(GTK_GRID(grid), button_eliminate, 1, row++, 1, 1);
    g_signal_connect(button_eliminate, "clicked", G_CALLBACK(eliminar), NULL);
    gtk_widget_set_visible(button_eliminate, FALSE);

   

    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv) {
    GtkApplication *app = gtk_application_new("org.ejemplo.filtro_procesos", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}
