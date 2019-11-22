#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include "interface.h"

GtkWidget *window;
GtkButton *btn_run;
GtkButton *btn_credits;
GtkBuilder *builder;
GtkFileChooser *fileChooserButton;
GtkImage   *img_selected;
GtkEntry *gtkEntry;

void btn_run_clicked() {
    printf("btn_run_clicked, we must call here the segmentation.\n");
    printf("%s\n", gtk_file_chooser_get_filename(fileChooserButton));
    printf("%s\n\n", gtk_entry_get_text(gtkEntry));
    gtk_image_set_from_file(img_selected, gtk_file_chooser_get_filename(fileChooserButton));
}

void btn_credits_clicked() {
    printf("btn_credits_clicked, we must show credits.\n\n");
}

void selection_changed() {
    printf("selection_changed\n\n");
    gtk_image_set_from_file(img_selected, gtk_file_chooser_get_filename(fileChooserButton));
}

void init_interface(int argc, char *argv[]) {
    // Init gtk
    gtk_init(&argc, &argv);

    // Get the builder
    builder = gtk_builder_new_from_file("interface.glade");

    // get the window
    window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));

    // Get the  buttons
    btn_run = GTK_BUTTON(gtk_builder_get_object(builder, "btn_run"));
    btn_credits = GTK_BUTTON(gtk_builder_get_object(builder, "btn_credits"));

    // Get the file choose
    fileChooserButton = GTK_FILE_CHOOSER(gtk_builder_get_object(builder, "file_chooser"));
    gtkEntry = GTK_ENTRY(gtk_builder_get_object(builder, "output_entry"));
    img_selected = GTK_IMAGE(gtk_builder_get_object(builder, "img_selected"));

    // Connects windows and buttons
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(btn_run, "clicked", G_CALLBACK(btn_run_clicked), NULL);
    g_signal_connect(btn_credits, "clicked", G_CALLBACK(btn_credits_clicked), NULL);
    g_signal_connect(fileChooserButton, "selection_changed", G_CALLBACK(selection_changed), NULL);

    gtk_builder_connect_signals(builder, NULL);
    gtk_widget_show(window);
    gtk_main();
}