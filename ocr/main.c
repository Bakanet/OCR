#include <gtk/gtk.h>
#include <SDL/SDL.h>
#include <stdio.h>
#include <err.h>
#include "mainOCR.h"

GtkWidget       *window_about;
GtkWidget       *window_main;
GtkWidget       *selection_window;
GtkImage 	*display_image;
GtkTextView		*text_view;
GtkFileChooser	*FileChooserButton;



//Launch the GUI and all the funtcions needed to make it work
int main(int argc, char *argv[])
{
    GtkBuilder *builder;

    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "window_main.glade", NULL);

    window_main = 
        GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));
	window_about 
	    = GTK_WIDGET(gtk_builder_get_object(builder, "window_about"));
	selection_window 
	    = GTK_WIDGET(gtk_builder_get_object(builder, "selection_window"));
												
	gtk_builder_connect_signals(builder, NULL);
    g_object_unref(builder);

    gtk_widget_show(window_main);
    gtk_main();

    return 0;
}





// Build and show About Windo/w
void window_about_open()
{
    GtkBuilder      *builder;

    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "window_main.glade", NULL);
    window_about 
        = GTK_WIDGET(gtk_builder_get_object(builder, "window_about"));
    gtk_window_set_transient_for(GTK_WINDOW(window_about),
        GTK_WINDOW(window_main));

    g_object_unref(builder);

    gtk_widget_show(window_about);
    gtk_main();
}





// Build and show Selection Window
void selection_window_open()
{
    GtkBuilder      *builder;

    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "window_main.glade", NULL);
    selection_window 
        = GTK_WIDGET(gtk_builder_get_object(builder, "selection_window"));
    gtk_window_set_transient_for(GTK_WINDOW(window_main),
        GTK_WINDOW(selection_window));
                                    
    display_image 
        = GTK_IMAGE(gtk_builder_get_object(builder, "display_image"));
	text_view 
	    = GTK_TEXT_VIEW(gtk_builder_get_object(builder, "text_view"));
	FileChooserButton 
	    = GTK_FILE_CHOOSER(gtk_builder_get_object(builder, "file_chooser"));
	gtk_builder_connect_signals(builder, NULL);											                                
    g_object_unref(builder);

    gtk_widget_show(selection_window);
    gtk_main();
}





// Get file name from File Selection Dialog and display image
void on_file_chooser_file_set()
{
	char *filename;
	
	filename = gtk_file_chooser_get_filename(FileChooserButton);
	
	gtk_image_set_from_file (display_image,	filename);
}




//Launch the function "TrainNetwork
void on_train_network_clicked()
{
	trainNetwork();
}




//Launch the entire OCR
void on_launch_button_clicked()
{
	launchOCR();
	
	FILE *filename = fopen("FilesTreatement/textOCR", "r");
	
	size_t max_size = 50000;
	char *s = (char *) malloc(sizeof(char) * max_size);
	if (!s)
		errx(1, "malloc failed");
	
	size_t i = 0;
	char cur;
	
	if (filename != NULL)
	{
		cur = fgetc(filename);
		while(cur != EOF)
		{
			s[i] = cur;
			i++;
			if (i >= max_size)
				break;
			cur = fgetc(filename);
		}
	}
	if (s!= NULL)
    {
        GtkTextBuffer *buffer = gtk_text_buffer_new(NULL);
        gtk_text_buffer_set_text(buffer, s, -1);
        gtk_text_view_set_buffer(text_view, buffer);
    } 
    free(s);
	fclose(filename);
}




// Close windows and quit GTK
void on_window_main_destroy()
{
    gtk_main_quit();
}
