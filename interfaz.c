#include <gtk/gtk.h>

GtkWidget *entry_pid, *entry_uid, *entry_gid, *entry_nombre, *entry_ppid, *entry_gpid, *entry_ram;
GtkWidget *dropdown_estado;
GtkWidget *textview;
GtkWidget *estado_buttons[5];
const char *estados[] = {"Running", "Sleeping", "Zombie", "T (Suspendido)", "D (Espera)"};
gint estado_seleccionado = 0;

static void seleccionar_estado(GtkButton *button, gpointer user_data) {
    gint index = GPOINTER_TO_INT(user_data);
    estado_seleccionado = index;
    for (int i = 0; i < 5; ++i) {
        gtk_widget_set_sensitive(estado_buttons[i], TRUE);
    }
    gtk_widget_set_sensitive(GTK_WIDGET(button), FALSE);
}

static void aplicar_filtros(GtkButton *button, gpointer user_data) {
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));
    gtk_text_buffer_set_text(buffer, "", -1);

   
    char *pid_text    = g_strdup(gtk_editable_get_text(GTK_EDITABLE(entry_pid)));
    char *uid_text    = g_strdup(gtk_editable_get_text(GTK_EDITABLE(entry_uid)));
    char *gid_text    = g_strdup(gtk_editable_get_text(GTK_EDITABLE(entry_gid)));
    char *nombre_text = g_strdup(gtk_editable_get_text(GTK_EDITABLE(entry_nombre)));
    char *ppid_text   = g_strdup(gtk_editable_get_text(GTK_EDITABLE(entry_ppid)));
    char *gpid_text   = g_strdup(gtk_editable_get_text(GTK_EDITABLE(entry_gpid)));
    char *ram_text    = g_strdup(gtk_editable_get_text(GTK_EDITABLE(entry_ram)));

    char *pid    = g_strstrip(pid_text);
    char *uid    = g_strstrip(uid_text);
    char *gid    = g_strstrip(gid_text);
    char *nombre = g_strstrip(nombre_text);
    char *ppid   = g_strstrip(ppid_text);
    char *gpid   = g_strstrip(gpid_text);
    char *ram    = g_strstrip(ram_text);

    GListModel *estado_model = gtk_drop_down_get_model(GTK_DROP_DOWN(dropdown_estado));
    guint index = gtk_drop_down_get_selected(GTK_DROP_DOWN(dropdown_estado));
    GtkStringObject *item = GTK_STRING_OBJECT(g_list_model_get_item(estado_model, index));
    const char *estado = gtk_string_object_get_string(item);
}
static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Filtro de Procesos");
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 500);

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
    ADD_ENTRY("Límite RAM:", entry_ram);


    for (int i = 0; i < 5; ++i) {
        estado_buttons[i] = gtk_button_new_with_label(estados[i]);
        g_signal_connect(estado_buttons[i], "clicked", G_CALLBACK(seleccionar_estado), GINT_TO_POINTER(i));
        gtk_grid_attach(GTK_GRID(grid), estado_buttons[i], i % 2, row + i / 2 + 1, 1, 1);
    }
    gtk_widget_set_sensitive(estado_buttons[0], FALSE);
    row += 4;


    GtkWidget *boton = gtk_button_new_with_label("Aplicar Filtros");
    gtk_grid_attach(GTK_GRID(grid), boton, 0, row++, 2, 1);
    g_signal_connect(boton, "clicked", G_CALLBACK(aplicar_filtros), NULL);

    textview = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(textview), FALSE);
    gtk_grid_attach(GTK_GRID(grid), textview, 0, row++, 2, 1);

    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv) {
    GtkApplication *app = gtk_application_new("org.ejemplo.filtro_moderno", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}
