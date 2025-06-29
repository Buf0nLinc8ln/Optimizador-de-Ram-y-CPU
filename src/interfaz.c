#include <ctype.h>
#include <string.h>
#include <gtk/gtk.h>
#include "../include/interfaz.h"

GtkWidget *entry_pid, *entry_uid, *entry_gid, *entry_nombre, *entry_ppid, *entry_gpid, *entry_estado;
GtkWidget *estado_buttons[5];
gint estado_seleccionado = 0;
GtkWidget *entry_ram_limit;
GtkWidget *entry_cpu_limit;
GtkWidget *button_suspend;
GtkWidget *button_eliminate;


int es_entero(char *cadena) {
    for (int i = 0; i < strlen(cadena); i++) {
        if (isdigit(cadena[i]) == 0) { // isdigt retorna 0 si el caracter no es un dígito
            return 0; 
        }
    }
    return 1; // retorna 1 si todos los caracteres son dígitos
}


void aplicar_filtros(GtkButton *button, gpointer user_data) {

    Global *global = (Global *)user_data;
    int primer_filtro = 0; // para indicar si es o no el primer filtro aplicado

    char *pid_text = g_strstrip(g_strdup(gtk_editable_get_text(GTK_EDITABLE(entry_pid))));
    char *uid_text = g_strstrip(g_strdup(gtk_editable_get_text(GTK_EDITABLE(entry_uid))));
    char *gid_text = g_strstrip(g_strdup(gtk_editable_get_text(GTK_EDITABLE(entry_gid))));
    char *nombre = g_strstrip(g_strdup(gtk_editable_get_text(GTK_EDITABLE(entry_nombre))));
    char *ppid_text = g_strstrip(g_strdup(gtk_editable_get_text(GTK_EDITABLE(entry_ppid))));
    char *gpid_text = g_strstrip(g_strdup(gtk_editable_get_text(GTK_EDITABLE(entry_gpid))));
    char *estado = g_strstrip(g_strdup(gtk_editable_get_text(GTK_EDITABLE(entry_estado))));

    // si en algun campo se ingresa "cualquier", se asigna un valor de 0 para int y 'none' para char

    if ((strcmp(pid_text, "cualquier") == 0)) {
        global->filtros.pid = 0; 
    } else {
        int pid = atoi(pid_text); // "atoi" convierte texto a entero
        global->filtros.pid = pid;
    }

    // para el campo UID
    if (strcmp(uid_text, "cualquier") == 0) {
        global->filtros.uid = 0; 
        global->filtros.user = "none";  
    } else if (es_entero(uid_text)) {
        global->filtros.uid = atoi(uid_text);
        global->filtros.user = NULL;
        if (primer_filtro == 0) {
            global->lista = filtrar_por_uid(NULL, global->filtros.uid, NULL, 1);
            if (global->lista == NULL) {
                gtk_label_set_markup(GTK_LABEL(label_uid_warning), "<span foreground='red'>*</span>"); // para poner un asterisco en rojo al lado del campo de texto
            } else {
                primer_filtro = 1;
                gtk_label_set_markup(GTK_LABEL(label_uid_warning), ""); // para quitar el asterisco en rojo al lado del campo de texto
            }
            
        } else {
            filtrar_por_uid(&global->lista, global->filtros.uid, NULL, 0);
        }
    } else {
        global->filtros.user = uid_text;
        global->filtros.uid = -1;
        if (primer_filtro == 0) {
            global->lista = filtrar_por_uid(NULL, -1, global->filtros.user, 1);
            if (global->lista == NULL) {
                gtk_label_set_markup(GTK_LABEL(label_uid_warning), "<span foreground='red'>*</span>"); 
            } else {
                primer_filtro = 1;
                gtk_label_set_markup(GTK_LABEL(label_uid_warning), ""); 
            }
        } else {
            filtrar_por_uid(&global->lista, -1, global->filtros.user, 0);
        }
    }
    
    // para el campo GID
    if (strcmp(gid_text, "cualquier") == 0) {
        global->filtros.gid = 0; 
        global->filtros.group = "none";  
    } else if (es_entero(gid_text)) {
        global->filtros.gid = atoi(gid_text);
        global->filtros.group = NULL;
        if (primer_filtro == 0) {
            global->lista = filtrar_por_gid(NULL, global->filtros.gid, NULL, 1);
            if (global->lista == NULL) {
                gtk_label_set_markup(GTK_LABEL(label_uid_warning), "<span foreground='red'>*</span>"); 
            } else {
                primer_filtro = 1;
                gtk_label_set_markup(GTK_LABEL(label_uid_warning), ""); 
            }
        } else {
            filtrar_por_gid(&global->lista, global->filtros.gid, NULL, 0);
        }
    } else {
        global->filtros.group = gid_text;
        global->filtros.gid = -1;
        if (primer_filtro == 0) {
            global->lista = filtrar_por_gid(NULL, -1, global->filtros.group, 1);
            if (global->lista == NULL) {
                gtk_label_set_markup(GTK_LABEL(label_uid_warning), "<span foreground='red'>*</span>"); 
            } else {
                primer_filtro = 1;
                gtk_label_set_markup(GTK_LABEL(label_uid_warning), ""); 
            }
        } else {
            filtrar_por_gid(&global->lista, -1, global->filtros.group, 0);
        }
    }

    // para el campo Nombre
    if (strcmp(nombre, "cualquier") == 0) {
        global->filtros.nombre = "none"; 
    } else {
        global->filtros.nombre = nombre;
        if (primer_filtro == 0) {
            global->lista = filtrar_por_nombre(NULL, global->filtros.nombre, 1);
            if (global->lista == NULL) {
                gtk_label_set_markup(GTK_LABEL(label_uid_warning), "<span foreground='red'>*</span>"); 
            } else {
                primer_filtro = 1;
                gtk_label_set_markup(GTK_LABEL(label_uid_warning), ""); 
            }
        } else {
            filtrar_por_nombre(&global->lista, global->filtros.nombre, 0);
        }
    }

    // para el campo PPID
    if (strcmp(ppid_text, "cualquier") == 0) {
        global->filtros.ppid = 0; 
    } else {
        global->filtros.ppid = atoi(ppid_text);
        if (primer_filtro == 0) {
            global->lista = filtrar_por_ppid(NULL, global->filtros.ppid, 1);
            if (global->lista == NULL) {
                gtk_label_set_markup(GTK_LABEL(label_uid_warning), "<span foreground='red'>*</span>"); 
            } else {
                primer_filtro = 1;
                gtk_label_set_markup(GTK_LABEL(label_uid_warning), ""); 
            }
        } else {
            filtrar_por_ppid(&global->lista, global->filtros.ppid, 0);
        }
    }

    // para el campo GPID
    if (strcmp(gpid_text, "cualquier") == 0) {
        global->filtros.gpid = 0; 
    } else {
        global->filtros.gpid = atoi(gpid_text);
        if (primer_filtro == 0) {
            global->lista = filtrar_por_gpid(NULL, global->filtros.gpid, 1);
            if (global->lista == NULL) {
                gtk_label_set_markup(GTK_LABEL(label_uid_warning), "<span foreground='red'>*</span>"); 
            } else {
                primer_filtro = 1;
                gtk_label_set_markup(GTK_LABEL(label_uid_warning), ""); 
            }
        } else {
            filtrar_por_gpid(&global->lista, global->filtros.gpid, 0);
        }
    }   

    // para el campo Estado
    if (strcmp(estado, "cualquier") == 0) {
        global->filtros.estado = 'n'; 
    } else {
        global->filtros.estado = estado[0];
        if (primer_filtro == 0) {
            global->lista = filtrar_por_estado(NULL, global->filtros.estado, 1);
            if (global->lista == NULL) {
                gtk_label_set_markup(GTK_LABEL(label_uid_warning), "<span foreground='red'>*</span>"); 
            } else {
                primer_filtro = 1;
                gtk_label_set_markup(GTK_LABEL(label_uid_warning), ""); 
            }
        } else {
            filtrar_por_estado(&global->lista, global->filtros.estado, 0);
        }
    }
}


void activar_modo(GtkButton *button, gpointer user_data) {
    const gchar *modo = (const gchar *)user_data;
    Global *global = (Global *)user_data;
    

    gtk_widget_set_visible(entry_ram_limit, g_strcmp0(modo, "RAM") == 0);
    gtk_widget_set_visible(entry_cpu_limit, g_strcmp0(modo, "CPU") == 0);

    gtk_widget_set_visible(button_suspend, TRUE);
    gtk_widget_set_visible(button_eliminate, TRUE);
}


void suspender(GtkButton *button, gpointer user_data) {
     g_print("Procesos suspendidos");
}


void eliminar(GtkButton *button, gpointer user_data) {
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

void activate(GtkApplication *app, gpointer user_data) {

    Global *datos = g_new(Global, 1);

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
    g_signal_connect(boton, "clicked", G_CALLBACK(aplicar_filtros( )), NULL);

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

