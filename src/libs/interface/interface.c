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

PresentationState dev_mode = {
        .size = 6,
        .is_active = 0,
        .data = {0, 0, 0, 0, 0, 0}
};


void init_interface(int argc, char *argv[]) {
    // Init gtk
    gtk_init(&argc, &argv);

    // Get the builder tool from the file
    builder = gtk_builder_new_from_file("src/libs/interface/interface.glade");

    // Get the 2 window (main window and result)
    main_window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));
    result_window = GTK_WIDGET(gtk_builder_get_object(builder, "result_window"));

    // Get the  buttons of the home window
    btn_run = GTK_BUTTON(gtk_builder_get_object(builder, "btn_run"));

    // Get the 2 buttons of the result page
    result_btn_cancel = GTK_BUTTON(gtk_builder_get_object(builder, "result_btn_cancel"));
    result_btn_save = GTK_BUTTON(gtk_builder_get_object(builder, "result_btn_save"));

    // Disable the button in main window
    gtk_widget_set_sensitive(GTK_WIDGET(btn_run), FALSE);


    textView = GTK_TEXT_VIEW(gtk_builder_get_object(builder, "text_view"));
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
void btn_run_clicked() {
    // Show the spinner to explain that the process has started
    gtk_spinner_start(home_spinner);

    // Set the button to not clickable, to be sure that nobody will click on it
    gtk_widget_set_sensitive(GTK_WIDGET(btn_run), FALSE);

    // If the toggle button is actived
    if (gtk_toggle_button_get_active(home_show_steps_check_btn) == TRUE) {
        dev_mode.is_active = 1;
        for (int i = 0; i < dev_mode.size; i++) {
            dev_mode.data[i] = 1;
        }
    }

    // Get the file to do the segmentation
    char *path = gtk_file_chooser_get_filename(fileChooserButton);

    // Get the content, by build the list
    char *content = build_text(path);

    // Set the content of the text of the result window
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(textView);
    gtk_text_buffer_set_text(buffer, content, -1);
    gtk_text_view_set_buffer(textView, buffer);

    // Turn off the spinner
    gtk_spinner_stop(home_spinner);

    // Show the result window
    gtk_widget_show(GTK_WIDGET(result_window));

    // Hide the first window
    gtk_widget_hide(GTK_WIDGET(main_window));
}


// Ok
void home_selection_changed2() {
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

void home_selection_changed() {
    // Get the new image from the file chooser picker
    char *path = gtk_file_chooser_get_filename(fileChooserButton);

    GtkImage *img = GTK_IMAGE(gtk_image_new_from_file (path));
    GdkPixbuf *pixbuf = gtk_image_get_pixbuf(img);

    pixbuf = gdk_pixbuf_scale_simple(pixbuf, 480, 360, GDK_INTERP_BILINEAR);

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