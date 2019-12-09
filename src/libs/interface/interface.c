#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include "interface.h"
#include "../image_segmentation/index.h"
#include "../dictionary_correction/dictionary.h"

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
GtkButton *home_close_button;

// The spinner on the home screen
GtkSpinner *home_spinner;

// the 3 toggle btn
GtkToggleButton *home_show_steps_check_btn;
GtkToggleButton *home_rotate_image_check_btn;
GtkToggleButton *home_save_letters_check_btn;
GtkToggleButton *home_remove_bg_check_btn;

PresentationState dev_mode = {
        .size = 6,
        .is_active = 0,
        .data = {0, 0, 0, 0, 0, 0}
};

PresentationState *presentationState = &dev_mode;

int must_save_letter = 0;
int must_rotate_image = 0;
int must_remove_bg = 0;

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
    gtk_text_view_set_wrap_mode(textView, GTK_WRAP_WORD_CHAR);
    fileChooserButton = GTK_FILE_CHOOSER(gtk_builder_get_object(builder, "file_chooser"));
    img_selected = GTK_IMAGE(gtk_builder_get_object(builder, "img_selected"));
    home_spinner = GTK_SPINNER(gtk_builder_get_object(builder, "home_spinner"));
    home_show_steps_check_btn = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "home_show_steps_check_btn"));
    home_rotate_image_check_btn = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "home_rotate_image_check_btn"));
    home_save_letters_check_btn = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "home_save_letters_check_btn"));
    home_remove_bg_check_btn = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "home_remove_bg_check_btn"));
    home_close_button = GTK_BUTTON(gtk_builder_get_object(builder, "home_close_button"));

    // Connects windows and buttons
    g_signal_connect(main_window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(btn_run, "clicked", G_CALLBACK(btn_run_clicked), NULL);
    g_signal_connect(fileChooserButton, "selection_changed", G_CALLBACK(home_selection_changed), NULL);
    g_signal_connect(result_btn_cancel, "clicked", G_CALLBACK(result_btn_cancel_clicked), NULL);
    g_signal_connect(result_btn_save, "clicked", G_CALLBACK(result_btn_save_clicked), NULL);
    g_signal_connect(home_close_button, "clicked", G_CALLBACK(gtk_main_quit), NULL);

    gtk_builder_connect_signals(builder, NULL);
    gtk_widget_show(main_window);
    gtk_main();
}

GtkTextBuffer *get_colorized_text(char *text) {
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(textView);
    GtkTextIter start;
    GtkTextIter end;
    gtk_text_buffer_get_start_iter(buffer, &start);
    gtk_text_buffer_get_end_iter(buffer, &end);
    gtk_text_buffer_delete(buffer, &start, &end);

    int i = 0;
    char *temp = calloc(10000, sizeof(char));
    while (text[i] != 0) {
        if (text[i] == ' ' || text[i] == '\n') {
            if (!check_in_dictionary(temp)) {
                char *word = calloc(1000, sizeof(char));
                strcat(word, "<span color=\"red\">");
                strcat(word, temp);
                strcat(word, "</span>");
                char *w = calloc(2, sizeof(char));
                w[0] = text[i];
                w[1] = 0;
                strcat(word, w);
                gtk_text_buffer_insert_markup(buffer, &end, word, -1);

            } else {
                char *word = calloc(1000, sizeof(char));
                strcat(word, temp);
                char *w = calloc(2, sizeof(char));
                w[0] = text[i];
                w[1] = 0;
                strcat(word, w);
                gtk_text_buffer_insert(buffer, &end, word, -1);
            }

            free(temp);
            temp = calloc(10000, sizeof(char));
        } else {
            char *w = calloc(2, sizeof(char));
            w[0] = text[i];
            w[1] = 0;
            strcat(temp, w);
        }
        i++;
    }
    return buffer;
}

// Ok
void btn_run_clicked() {
    // Show the spinner to explain that the process has started
    gtk_spinner_start(home_spinner);

    // Set the button to not clickable, to be sure that nobody will click on it
    gtk_widget_set_sensitive(GTK_WIDGET(btn_run), FALSE);

    // If the toggle button is actived
    if (gtk_toggle_button_get_active(home_show_steps_check_btn) == TRUE) {
        presentationState->is_active = 1;
        presentationState->data[0] = 1;
        presentationState->data[1] = 2;
    } else {
        presentationState->is_active = 0;
        for (int i = 0; i < presentationState->size; i++) {
            presentationState->data[i] = 0;
        }
    }
    
    must_save_letter = gtk_toggle_button_get_active(home_save_letters_check_btn);
    must_rotate_image = gtk_toggle_button_get_active(home_rotate_image_check_btn);
    must_remove_bg = gtk_toggle_button_get_active(home_remove_bg_check_btn);

    // Get the file to do the segmentation
    char *path = gtk_file_chooser_get_filename(fileChooserButton);

    // Get the content, by build the list
    char *content = build_text(path);

    // Set the content of the text of the result window



    GtkTextBuffer *buffer = get_colorized_text(content);

    gtk_text_view_set_buffer(textView, buffer);


    // Turn off the spinner
    gtk_spinner_stop(home_spinner);

    // Show the result window
    gtk_widget_show(GTK_WIDGET(result_window));

    // Hide the first window
    gtk_widget_hide(GTK_WIDGET(main_window));
}


void home_selection_changed() {
    // Get the new image from the file chooser picker
    char *path = gtk_file_chooser_get_filename(fileChooserButton);

    GtkImage *img = GTK_IMAGE(gtk_image_new_from_file(path));
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
    // gtk_widget_set_sensitive(GTK_WIDGET(btn_run), TRUE);
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