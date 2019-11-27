#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include "interface.h"
#include "../image_segmentation/index.h"

// The all window
GtkWidget *main_window;
GtkWidget *result_window;

// The run btn
GtkButton *btn_run;

GtkButton *btn_credits;
GtkBuilder *builder;
GtkFileChooser *fileChooserButton;
GtkImage *img_selected;
GtkTextView *textView;

GtkButton *btn_result_cancel;
GtkButton *btn_result_save;

void btn_run_clicked() {
    // Get the path from the btn
    char *path = gtk_file_chooser_get_filename(fileChooserButton);

    // Show Loader
    // TODO SHOW LOADER

    // Hide loader
    // TODO HIDE LOADER

    char *content = build_text(path);
    printf("%s", content);
    gtk_widget_hide(main_window);
    gtk_widget_show(result_window);
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(textView);
    gtk_text_buffer_set_text(buffer, content, -1);
    gtk_text_view_set_buffer(textView, buffer);


}

void btn_credits_clicked() {
    printf("btn_credits_clicked, we must show credits.\n\n");
}

void btn_result_cancel_clicked() {
    gtk_widget_hide(result_window);
    gtk_widget_show(main_window);
}

void btn_result_save_clicked() {
    char *filename = calloc(10000, sizeof(char));
    int state = gtk_get_file_to_save(&filename);

    if (state == 1) {
        GtkTextIter start, end;

        GtkTextBuffer *buffer = gtk_text_view_get_buffer(textView);

        gtk_text_buffer_get_start_iter(buffer, &start);
        gtk_text_buffer_get_end_iter(buffer, &end);


        gchar *content = gtk_text_buffer_get_text(buffer, &start, &end, 0);
        FILE *ptr = fopen(filename, "w");
        fprintf(ptr, "%s", content);
        fclose(ptr);
        gtk_widget_hide(result_window);
        gtk_widget_show(main_window);
    }

}

void selection_changed() {
    printf("selection_changed\n\n");
    gtk_image_set_from_file(img_selected, gtk_file_chooser_get_filename(fileChooserButton));
    gtk_widget_set_sensitive(GTK_WIDGET(btn_run), TRUE);
}

int gtk_get_file_to_save(char **filename) {
    GtkWidget *dialog;
    GtkFileChooser *chooser;
    gint res;

    dialog = gtk_file_chooser_dialog_new("Save File",
                                         GTK_WINDOW(main_window),
                                         GTK_FILE_CHOOSER_ACTION_SAVE,
                                         "Cancel",
                                         GTK_RESPONSE_CANCEL,
                                         "_Save",
                                         GTK_RESPONSE_ACCEPT,
                                         NULL);
    chooser = GTK_FILE_CHOOSER(dialog);

    gtk_file_chooser_set_do_overwrite_confirmation(chooser, TRUE);
    res = gtk_dialog_run(GTK_DIALOG(dialog));
    if (res == -3) {
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
        strcpy(*filename, gtk_file_chooser_get_filename(chooser));
        gtk_widget_destroy(dialog);
        return 1;
    }
    gtk_widget_destroy(dialog);
    return 0;
}

void init_interface(int argc, char *argv[]) {
    // Init gtk
    gtk_init(&argc, &argv);

    // Get the builder
    builder = gtk_builder_new_from_file("src/libs/interface/interface.glade");

    // get the window
    main_window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));
    result_window = GTK_WIDGET(gtk_builder_get_object(builder, "result_window"));

    // Get the  buttons
    btn_run = GTK_BUTTON(gtk_builder_get_object(builder, "btn_run"));
    btn_credits = GTK_BUTTON(gtk_builder_get_object(builder, "btn_credits"));
    btn_result_cancel = GTK_BUTTON(gtk_builder_get_object(builder, "btn_result_cancel"));
    btn_result_save = GTK_BUTTON(gtk_builder_get_object(builder, "btn_result_save"));
    gtk_widget_set_sensitive(GTK_WIDGET(btn_run), FALSE);
    textView = GTK_TEXT_VIEW(gtk_builder_get_object(builder, "text_view"));
    // Get the file choose
    fileChooserButton = GTK_FILE_CHOOSER(gtk_builder_get_object(builder, "file_chooser"));
    img_selected = GTK_IMAGE(gtk_builder_get_object(builder, "img_selected"));


    // Connects windows and buttons
    g_signal_connect(main_window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(btn_run, "clicked", G_CALLBACK(btn_run_clicked), NULL);
    g_signal_connect(btn_credits, "clicked", G_CALLBACK(btn_credits_clicked), NULL);
    g_signal_connect(fileChooserButton, "selection_changed", G_CALLBACK(selection_changed), NULL);
    g_signal_connect(btn_result_cancel, "clicked", G_CALLBACK(btn_result_cancel_clicked), NULL);
    g_signal_connect(btn_result_save, "clicked", G_CALLBACK(btn_result_save_clicked), NULL);

    gtk_builder_connect_signals(builder, NULL);
    gtk_widget_show(main_window);


    gtk_main();
}