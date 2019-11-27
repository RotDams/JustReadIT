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
GtkImage   *img_selected;
GtkTextView *textView;

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

void selection_changed() {
    printf("selection_changed\n\n");
    gtk_image_set_from_file(img_selected, gtk_file_chooser_get_filename(fileChooserButton));
    gtk_widget_set_sensitive(GTK_WIDGET(btn_run), TRUE);
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

    gtk_builder_connect_signals(builder, NULL);
    gtk_widget_show(main_window);
    gtk_main();
}