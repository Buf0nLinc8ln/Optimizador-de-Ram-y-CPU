#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <gtk/gtk.h>
#include "../include/interfaz.h"
#include "../include/Filtros.h"
#include "../include/control.h"

GtkWidget *entry_pid, *entry_uid, *entry_gid, *entry_nombre, *entry_ppid, *entry_gpid, *entry_estado;
GtkWidget *estado_buttons[5];
gint estado_seleccionado = 0;
GtkWidget *entry_ram_limit;
GtkWidget *entry_cpu_limit;
GtkWidget *button_suspend;
GtkWidget *button_eliminate;
# define INDICE_FILTROS 9
GtkWidget *asterisco[INDICE_FILTROS] = {NULL};

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
        if (global->primer_filtro == 0) {
            global->lista = filtrar_por_pid(global->filtros.pid);
            if (global->lista == NULL) {
                gtk_label_set_markup(GTK_LABEL(asterisco[0]), "<span foreground='red'>*</span>"); 
            } else {
                global->primer_filtro = 1;
                gtk_label_set_markup(GTK_LABEL(asterisco[0]), ""); 
                global->filtrado_por_pid = 1;
                imprimirLista(global->lista);
                printf("\n");
            }
        } 
    }

    
    
    if (global->filtrado_por_pid != 1) {

        // para el campo UID
        if (strcmp(uid_text, "cualquier") == 0) {
            global->filtros.uid = 0; 
            global->filtros.user = "none";  
        } else if (es_entero(uid_text)) {
            global->filtros.uid = atoi(uid_text);
            global->filtros.user = NULL;
            if (global->primer_filtro == 0) {
                global->lista = filtrar_por_uid(NULL, global->filtros.uid, NULL, 1);
                if (global->lista == NULL) {
                    if (asterisco[1] != NULL) {
                        gtk_label_set_markup(GTK_LABEL(asterisco[1]), "<span foreground='red'>*</span>"); // para poner un asterisco en rojo al lado del campo de texto
                    }
                } else {
                    global->primer_filtro = 1;
                    if (asterisco[1]) {
                        gtk_label_set_markup(GTK_LABEL(asterisco[1]), ""); // para quitar el asterisco en rojo al lado del campo de texto
                        imprimirLista(global->lista);
                        printf("\n");
                    }
                }
                
            } else {
                filtrar_por_uid(&global->lista, global->filtros.uid, NULL, 0);
                imprimirLista(global->lista);
                printf("\n");
            }
        } else {
            global->filtros.user = uid_text;
            global->filtros.uid = -1;
            if (global->primer_filtro == 0) {
                global->lista = filtrar_por_uid(NULL, -1, global->filtros.user, 1);
                if (global->lista == NULL) {
                    gtk_label_set_markup(GTK_LABEL(asterisco[1]), "<span foreground='red'>*</span>"); 
                } else {
                    global->primer_filtro = 1;
                    gtk_label_set_markup(GTK_LABEL(asterisco[1]), "");
                    imprimirLista(global->lista);
                    printf("\n");
                }
            } else {
                filtrar_por_uid(&global->lista, -1, global->filtros.user, 0);
                imprimirLista(global->lista);
                printf("\n");
            }
        }
        
        // para el campo GID
        if (strcmp(gid_text, "cualquier") == 0) {
            global->filtros.gid = 0; 
            global->filtros.group = "none";  
        } else if (es_entero(gid_text)) {
            global->filtros.gid = atoi(gid_text);
            global->filtros.group = NULL;
            if (global->primer_filtro == 0) {
                global->lista = filtrar_por_gid(NULL, global->filtros.gid, NULL, 1);
                if (global->lista == NULL) {
                    gtk_label_set_markup(GTK_LABEL(asterisco[2]), "<span foreground='red'>*</span>"); 
                } else {
                    global->primer_filtro = 1;
                    gtk_label_set_markup(GTK_LABEL(asterisco[2]), "");
                    imprimirLista(global->lista);
                    printf("\n");
                }
            } else {
                filtrar_por_gid(&global->lista, global->filtros.gid, NULL, 0);
                imprimirLista(global->lista);
                printf("\n");
            }
        } else {
            global->filtros.group = gid_text;
            global->filtros.gid = -1;
            if (global->primer_filtro == 0) {
                global->lista = filtrar_por_gid(NULL, -1, global->filtros.group, 1);
                if (global->lista == NULL) {
                    gtk_label_set_markup(GTK_LABEL(asterisco[2]), "<span foreground='red'>*</span>"); 
                } else {
                    global->primer_filtro = 1;
                    gtk_label_set_markup(GTK_LABEL(asterisco[2]), "");
                    imprimirLista(global->lista);
                    printf("\n");
                }
            } else {
                filtrar_por_gid(&global->lista, -1, global->filtros.group, 0);
                imprimirLista(global->lista);
                printf("\n");
            }
        }

        // para el campo Nombre
        if (strcmp(nombre, "cualquier") == 0) {
            global->filtros.nombre = "none"; 
        } else {
            global->filtros.nombre = nombre;
            if (global->primer_filtro == 0) {
                global->lista = filtrar_por_nombre(NULL, global->filtros.nombre, 1);
                if (global->lista == NULL) {
                    gtk_label_set_markup(GTK_LABEL(asterisco[3]), "<span foreground='red'>*</span>"); 
                } else {
                    global->primer_filtro = 1;
                    gtk_label_set_markup(GTK_LABEL(asterisco[3]), "");
                    imprimirLista(global->lista);
                    printf("\n");
                }
            } else {
                filtrar_por_nombre(&global->lista, global->filtros.nombre, 0);
                imprimirLista(global->lista);
                printf("\n");
            }
        }

        // para el campo PPID
        if (strcmp(ppid_text, "cualquier") == 0) {
            global->filtros.ppid = 0; 
        } else {
            global->filtros.ppid = atoi(ppid_text);
            if (global->primer_filtro == 0) {
                global->lista = filtrar_por_ppid(NULL, global->filtros.ppid, 1);
                if (global->lista == NULL) {
                    gtk_label_set_markup(GTK_LABEL(asterisco[4]), "<span foreground='red'>*</span>"); 
                } else {
                    global->primer_filtro = 1;
                    gtk_label_set_markup(GTK_LABEL(asterisco[4]), "");
                    imprimirLista(global->lista);
                    printf("\n");
                }
            } else {
                filtrar_por_ppid(&global->lista, global->filtros.ppid, 0);
                imprimirLista(global->lista);
                printf("\n");
            }
        }

        // para el campo GPID
        if (strcmp(gpid_text, "cualquier") == 0) {
            global->filtros.gpid = 0; 
        } else {
            global->filtros.gpid = atoi(gpid_text);
            if (global->primer_filtro == 0) {
                global->lista = filtrar_por_gpid(NULL, global->filtros.gpid, 1);
                if (global->lista == NULL) {
                    gtk_label_set_markup(GTK_LABEL(asterisco[5]), "<span foreground='red'>*</span>"); 
                } else {
                    global->primer_filtro = 1;
                    gtk_label_set_markup(GTK_LABEL(asterisco[5]), "");
                    imprimirLista(global->lista);
                    printf("\n");
                }
            } else {
                filtrar_por_gpid(&global->lista, global->filtros.gpid, 0);
                imprimirLista(global->lista);
                printf("\n");
            }
        }   

        // para el campo Estado
        if (strcmp(estado, "cualquier") == 0) {
            global->filtros.estado = 'n'; 
        } else {
            global->filtros.estado = estado[0];
            if (global->primer_filtro == 0) {
                global->lista = filtrar_por_estado(NULL, global->filtros.estado, 1);
                if (global->lista == NULL) {
                    gtk_label_set_markup(GTK_LABEL(asterisco[6]), "<span foreground='red'>*</span>"); 
                } else {
                    global->primer_filtro = 1;
                    gtk_label_set_markup(GTK_LABEL(asterisco[6]), "");
                    imprimirLista(global->lista);
                    printf("\n");
                }
            } else {
                filtrar_por_estado(&global->lista, global->filtros.estado, 0);
                imprimirLista(global->lista);
                printf("\n");
            }
        }
    }
    // liberar memoria 
    g_free(pid_text);
    g_free(uid_text);
    g_free(gid_text);
    g_free(nombre);
    g_free(ppid_text);
    g_free(gpid_text);
    g_free(estado);
}


void activar_modo(GtkButton *button, gpointer user_data) {
    Global *global = (Global *)user_data;

    char *limite_ram_text = g_strstrip(g_strdup(gtk_editable_get_text(GTK_EDITABLE(entry_ram_limit))));
    char *limite_cpu_text = g_strstrip(g_strdup(gtk_editable_get_text(GTK_EDITABLE(entry_cpu_limit))));


    long limite_ram = atol(limite_ram_text); // texto a long
    float limite_cpu = atof(limite_cpu_text); // texto a float

    if (g_strcmp0(gtk_button_get_label(button),"Modo 'RAM'") == 0) {
        global->modo = "RAM";
    } else if (g_strcmp0(gtk_button_get_label(button),"Modo 'CPU'") == 0) {
        global->modo = "CPU";
    }


    gtk_widget_set_visible(entry_ram_limit, g_strcmp0(global->modo, "RAM") == 0);
    gtk_widget_set_visible(entry_cpu_limit, g_strcmp0(global->modo, "CPU") == 0);
    gtk_widget_set_visible(button_suspend, TRUE);
    gtk_widget_set_visible(button_eliminate, TRUE);

    // para el campo ram
    if (g_strcmp0(global->modo, "RAM") == 0) {
        if (strcmp(limite_ram_text, "cualquier") == 0) {
            global->filtros.limite_ram = 0;
        } else {
            global->filtros.limite_ram = limite_ram;
        }

        if (global->primer_filtro == 0) {
            global->lista = filtrar_por_ram(NULL, global->filtros.limite_ram, 1);
            if (global->lista == NULL) {
                gtk_label_set_markup(GTK_LABEL(asterisco[7]), "<span foreground='red'>*</span>");
            } else {
                global->primer_filtro = 1;
                gtk_label_set_text(GTK_LABEL(asterisco[7]), "");
                imprimirLista(global->lista);
                printf("\n");
            }
        } else {
            filtrar_por_ram(&global->lista, global->filtros.limite_ram, 0);
            imprimirLista(global->lista);
            printf("\n");
        }
    }

    // para el campo cpu
    if (g_strcmp0(global->modo, "CPU") == 0) {
        if (strcmp(limite_cpu_text, "cualquier") == 0) {
            global->filtros.limite_cpu = 0;
        } else {
            global->filtros.limite_cpu = limite_cpu;
        }

        if (global->primer_filtro == 0) {
            global->lista = filtrar_por_cpu(NULL, global->filtros.limite_cpu, 1);
            if (global->lista == NULL) {
                gtk_label_set_markup(GTK_LABEL(asterisco[8]), "<span foreground='red'>*</span>");
            } else {
                global->primer_filtro = 1;
                gtk_label_set_text(GTK_LABEL(asterisco[8]), "");
                imprimirLista(global->lista);
                printf("\n");
            }
        } else {
            filtrar_por_cpu(&global->lista, global->filtros.limite_cpu, 0);
            imprimirLista(global->lista);
            printf("\n");
        }
    }


    g_free(limite_ram_text);
    g_free(limite_cpu_text);
}


void suspender(GtkButton *button, gpointer user_data) {
    Global *global = (Global *)user_data;
    suspender_procesos(&global->lista);
}


void eliminar(GtkButton *button, gpointer user_data) {
    Global *global = (Global *)user_data;
    eliminar_procesos(&global->lista);
}

// void reanudar(GtkButton *button, gpointer user_data) {
//     Global *global = (Global *)user_data;
//     reanudar_procesos(&global->lista);
// }

void reiniciar_filtros(GtkButton *button, gpointer user_data) {
    Global *global = (Global *)user_data;

    // liberar memoria de la lista si existe
    if (global->lista != NULL) {
        freeLista(global->lista);
        global->lista = NULL;
    }

    global->primer_filtro = 0;
    global->filtrado_por_pid = 0;
    global->modo = NULL;

    // limpiar la estructura filtros, evitar errores
    memset(&(global->filtros), 0, sizeof(global->filtros));

    // se coloca "cualquiera" como predeterminado
    gtk_editable_set_text(GTK_EDITABLE(entry_pid), "cualquier");
    gtk_editable_set_text(GTK_EDITABLE(entry_uid), "cualquier");
    gtk_editable_set_text(GTK_EDITABLE(entry_gid), "cualquier");
    gtk_editable_set_text(GTK_EDITABLE(entry_nombre), "cualquier");
    gtk_editable_set_text(GTK_EDITABLE(entry_ppid), "cualquier");
    gtk_editable_set_text(GTK_EDITABLE(entry_gpid), "cualquier");
    gtk_editable_set_text(GTK_EDITABLE(entry_estado), "cualquier");
    gtk_editable_set_text(GTK_EDITABLE(entry_ram_limit), "cualquier");
    gtk_editable_set_text(GTK_EDITABLE(entry_cpu_limit), "cualquier");

    // ocultar otra vez campos 
    gtk_widget_set_visible(entry_ram_limit, FALSE);
    gtk_widget_set_visible(entry_cpu_limit, FALSE);
    gtk_widget_set_visible(button_suspend, FALSE);
    gtk_widget_set_visible(button_eliminate, FALSE);

    // quitar asterisco si lo hubiera
    gtk_label_set_text(GTK_LABEL(asterisco[0]), "");
    gtk_label_set_text(GTK_LABEL(asterisco[1]), "");
    gtk_label_set_text(GTK_LABEL(asterisco[2]), "");
    gtk_label_set_text(GTK_LABEL(asterisco[3]), "");
    gtk_label_set_text(GTK_LABEL(asterisco[4]), "");
    gtk_label_set_text(GTK_LABEL(asterisco[5]), "");
    gtk_label_set_text(GTK_LABEL(asterisco[6]), "");
    gtk_label_set_text(GTK_LABEL(asterisco[7]), "");
    gtk_label_set_text(GTK_LABEL(asterisco[8]), "");
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

// void reanudar(GtkWindow *window, Node **lista) {
//     reanudar_procesos(lista);
// }


// static void on_confirmar_dialog_response(GtkDialog *dialog, gint response_id, gpointer user_data) {
//     GtkWindow *window = GTK_WINDOW(user_data);
//     Global *global = g_object_get_data(G_OBJECT(dialog), "global");

//     if (response_id == GTK_RESPONSE_YES) {
//         reanudar(window, &global->lista);
//     }

//     if (global->lista != NULL) {
//         freeLista(global->lista);
//         global->lista = NULL;
//     }

//     g_free(global);
//     gtk_window_destroy(window);
//     gtk_window_destroy(GTK_WINDOW(dialog)); // cierra el diálogo
// }

// static gboolean confirmar_salida(GtkWindow *window, gpointer user_data) {
//     Global *global = (Global *)user_data;

//     if (global->suspendido == 1) {
//         GtkWidget *dialog = gtk_message_dialog_new(
//             window,
//             GTK_DIALOG_MODAL,
//             GTK_MESSAGE_QUESTION,
//             GTK_BUTTONS_YES_NO,
//             "¿Reanudar procesos?"
//         );

//         g_object_set_data(G_OBJECT(dialog), "global", global);
//         g_signal_connect(dialog, "response", G_CALLBACK(on_confirmar_dialog_response), window);
//         gtk_window_present(GTK_WINDOW(dialog));
//         return TRUE;
//     }

//     if (global->lista != NULL) {
//         freeLista(global->lista);
//         global->lista = NULL;
//     }

//     g_free(global);
//     gtk_window_destroy(window);
//     return TRUE;
// }

void activate(GtkApplication *app, gpointer user_data) {

    Global *global = g_new(Global, 1);

    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Optimizador de Ram y CPU");
    gtk_window_set_default_size(GTK_WINDOW(window), 500, 500);

    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 5);
    gtk_window_set_child(GTK_WINDOW(window), grid);

    int row = 0;
    #define ADD_ENTRY(label, widget, indice) \
        gtk_grid_attach(GTK_GRID(grid), gtk_label_new(label), 0, row, 1, 1); \
        widget = gtk_entry_new(); \
        gtk_grid_attach(GTK_GRID(grid), widget, 1, row++, 1, 1); \
        asterisco[indice] = gtk_label_new(""); \
        gtk_label_set_xalign(GTK_LABEL(asterisco[indice]), 0.0); \
        gtk_grid_attach(GTK_GRID(grid), asterisco[indice], 2, row++, 1, 1);

    ADD_ENTRY("PID:", entry_pid, 0);
    ADD_ENTRY("UID:", entry_uid, 1);
    ADD_ENTRY("GID:", entry_gid, 2);
    ADD_ENTRY("Nombre:", entry_nombre, 3);
    ADD_ENTRY("PPID:", entry_ppid, 4);
    ADD_ENTRY("GPID:", entry_gpid, 5);
    ADD_ENTRY("Estado:", entry_estado, 6);

    // inicie en "cualquier" al lanzar la app
    gtk_editable_set_text(GTK_EDITABLE(entry_pid), "cualquier");
    gtk_editable_set_text(GTK_EDITABLE(entry_uid), "cualquier");
    gtk_editable_set_text(GTK_EDITABLE(entry_gid), "cualquier");
    gtk_editable_set_text(GTK_EDITABLE(entry_nombre), "cualquier");
    gtk_editable_set_text(GTK_EDITABLE(entry_ppid), "cualquier");
    gtk_editable_set_text(GTK_EDITABLE(entry_gpid), "cualquier");
    gtk_editable_set_text(GTK_EDITABLE(entry_estado), "cualquier");


    // tooltip sirve para mostrar información adicional al pasar el cursor sobre el widget
    gtk_widget_set_tooltip_text(entry_estado, "Estados posibles: F (foreground), B (background), R (Running), S (Sleeping), Z (Zombie) T (Suspendido), D (Espera ininterrumpida)");

    
    GtkWidget *boton = gtk_button_new_with_label("Aplicar Filtros");
    gtk_grid_attach(GTK_GRID(grid), boton, 0, row, 1, 1);
    g_signal_connect(boton, "clicked", G_CALLBACK(aplicar_filtros), global);

    GtkWidget *boton_reiniciar = gtk_button_new_with_label("Reiniciar Filtros");
    gtk_grid_attach(GTK_GRID(grid), boton_reiniciar, 1, row++, 1, 1);
    g_signal_connect(boton_reiniciar, "clicked", G_CALLBACK(reiniciar_filtros), global);

    GtkWidget *separator = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_grid_attach(GTK_GRID(grid), separator, 0, row++, 2, 1);

    GtkWidget *button_modo_ram = gtk_button_new_with_label("Modo 'RAM'");
    gtk_grid_attach(GTK_GRID(grid), button_modo_ram, 0, row, 1, 1);
    g_signal_connect(button_modo_ram, "clicked", G_CALLBACK(activar_modo), global);

    GtkWidget *button_modo_cpu = gtk_button_new_with_label("Modo 'CPU'");
    gtk_grid_attach(GTK_GRID(grid), button_modo_cpu, 1, row++, 1, 1);
    g_signal_connect(button_modo_cpu, "clicked", G_CALLBACK(activar_modo), global);

    entry_ram_limit = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_ram_limit), "Límite RAM");
    gtk_grid_attach(GTK_GRID(grid), entry_ram_limit, 0, row++, 2, 1);
    asterisco[7] = gtk_label_new(""); 
    gtk_label_set_xalign(GTK_LABEL(asterisco[7]), 0.0); 
    gtk_grid_attach(GTK_GRID(grid), asterisco[7], 2, row++, 1, 1);
    gtk_widget_set_visible(entry_ram_limit, FALSE);
    gtk_editable_set_text(GTK_EDITABLE(entry_ram_limit), "cualquier");

    entry_cpu_limit = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_cpu_limit), "Límite CPU");
    gtk_grid_attach(GTK_GRID(grid), entry_cpu_limit, 0, row++, 2, 1);\
    asterisco[8] = gtk_label_new("");
    gtk_label_set_xalign(GTK_LABEL(asterisco[8]), 0.0); 
    gtk_grid_attach(GTK_GRID(grid), asterisco[8], 2, row++, 1, 1);
    gtk_widget_set_visible(entry_cpu_limit, FALSE);
    gtk_editable_set_text(GTK_EDITABLE(entry_cpu_limit), "cualquier");

    button_suspend = gtk_button_new_with_label("Suspender");
    gtk_grid_attach(GTK_GRID(grid), button_suspend, 0, row, 1, 1);
    g_signal_connect(button_suspend, "clicked", G_CALLBACK(suspender), global);
    gtk_widget_set_visible(button_suspend, FALSE);

    button_eliminate = gtk_button_new_with_label("Eliminar");
    gtk_grid_attach(GTK_GRID(grid), button_eliminate, 1, row++, 1, 1);
    g_signal_connect(button_eliminate, "clicked", G_CALLBACK(eliminar), global);
    gtk_widget_set_visible(button_eliminate, FALSE);

   
    // g_signal_connect(window, "close-request", G_CALLBACK(confirmar_salida), global);
    gtk_window_present(GTK_WINDOW(window));
}

