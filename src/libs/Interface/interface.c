#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include "interface.h"

void init_interface()
{
	GtkBuilder *builder=NULL; 
	GtkWidget *window=NULL;

	gtk_init();

	builder = gtk_builder_new();
	gtk_builder_add_from_file (builder, "interface.glade", NULL);

	window = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));
	gtk_builder_connect_signals(builder, NULL);

	g_object_unref(builder);

	gtk_widget_show(window);                
	gtk_main();
}

// Called when window is closed
void on_window_main_destroy()
{
	gtk_main_quit();
}

// Called when the user click on the source entry button
void choose_source_file()
{
	GtkWidget *dialog;
	GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
	gint res;

	dialog = gtk_file_chooser_dialog_new ("Open File",
			parent_window,
			action,
			_("_Cancel"),
			GTK_RESPONSE_CANCEL,
			_("_Open"),
			GTK_RESPONSE_ACCEPT,
			NULL);

	res = gtk_dialog_run (GTK_DIALOG (dialog));
	if (res == GTK_RESPONSE_ACCEPT)
	{
		char *filename;
		GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
		filename = gtk_file_chooser_get_filename (chooser);
		open_file (filename);
		g_free (filename);
	}

	gtk_widget_destroy (dialog);
}

// Called when the user decide to save the output of the OCR
{
	GtkWidget *dialog;
	GtkFileChooser *chooser;
	GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_SAVE;
	gint res;

	dialog = gtk_file_chooser_dialog_new ("Save File",
			parent_window,
			action,
			_("_Cancel"),
			GTK_RESPONSE_CANCEL,
			_("_Save"),
			GTK_RESPONSE_ACCEPT,
			NULL);
	chooser = GTK_FILE_CHOOSER (dialog);

	gtk_file_chooser_set_do_overwrite_confirmation (chooser, TRUE);

	if (user_edited_a_new_document)
		gtk_file_chooser_set_current_name (chooser,
				_("Untitled document"));
	else
		gtk_file_chooser_set_filename (chooser,
				existing_filename);

	res = gtk_dialog_run (GTK_DIALOG (dialog));
	if (res == GTK_RESPONSE_ACCEPT)
	{
		char *filename;

		filename = gtk_file_chooser_get_filename (chooser);
		save_to_file (filename);
		g_free (filename);
	}

	gtk_widget_destroy (dialog);
}
