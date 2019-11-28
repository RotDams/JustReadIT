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

GtkBuilder *builder;
GtkFileChooser *fileChooserButton;
GtkImage *img_selected;
GtkTextView *textView;

GtkButton *result_btn_cancel;
GtkButton *result_btn_save;

// The spinner on the home screen
GtkSpinner *home_spinner;

GtkToggleButton *home_show_steps_check_btn;

void btn_run_clicked() {
    // Get the path from the btn
    /*
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
    */


    //char *path = gtk_file_chooser_get_filename(fileChooserButton);
    //char *content = build_text(path);
    //printf("%s", content);

    // Show the spinner to explain that the process has started
    gtk_spinner_start(home_spinner);

    // Set the button to not clickable, to be sure that nobody will click on it
    gtk_widget_set_sensitive(GTK_WIDGET(btn_run), FALSE);

    // TODO CHECK THIS FUNCTION RETURN
    if (gtk_toggle_button_get_active(home_show_steps_check_btn) == 1)
        printf("test");

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
    result_btn_cancel = GTK_BUTTON(gtk_builder_get_object(builder, "result_btn_cancel"));
    result_btn_save = GTK_BUTTON(gtk_builder_get_object(builder, "result_btn_save"));
    gtk_widget_set_sensitive(GTK_WIDGET(btn_run), FALSE);
    textView = GTK_TEXT_VIEW(gtk_builder_get_object(builder, "text_view"));
    // Get the file choose
    fileChooserButton = GTK_FILE_CHOOSER(gtk_builder_get_object(builder, "file_chooser"));
    img_selected = GTK_IMAGE(gtk_builder_get_object(builder, "img_selected"));

    home_spinner = GTK_SPINNER(gtk_builder_get_object(builder, "home_spinner"));
    home_show_steps_check_btn = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "home_show_steps_check_btn"));

    // Connects windows and buttons
    g_signal_connect(main_window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(btn_run, "clicked", G_CALLBACK(btn_run_clicked), NULL);
    g_signal_connect(fileChooserButton, "selection_changed", G_CALLBACK(home_selection_changed), NULL);
    g_signal_connect(result_btn_cancel, "clicked", G_CALLBACK(result_btn_cancel_clicked), NULL);
    g_signal_connect(result_btn_save, "clicked", G_CALLBACK(result_btn_save_clicked), NULL);

    gtk_builder_connect_signals(builder, NULL);
    gtk_widget_show(main_window);


    gtk_main();
}



// Ok
void home_selection_changed() {
    // Get the new image from the file chooser picker
    char *path = gtk_file_chooser_get_filename(fileChooserButton);

    // Create an error variable to pass to the function witch get the image
    GError *gerror;

    // Create a pixpuf from a file. It will be 480x360 at max, and KEEP THE RATIO
    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file_at_size(path, 480, 360, &gerror);

    // Set the image on the screen
    gtk_image_set_from_pixbuf(img_selected, pixbuf);

    // We have at least 1 file. So we can and must active the button
    gtk_widget_set_sensitive(GTK_WIDGET(btn_run), TRUE);
}

// Ok
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

// Ok
void result_btn_cancel_clicked() {
    gtk_widget_hide(result_window);
    gtk_widget_show(main_window);
}

// Ok
void result_btn_save_clicked() {
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