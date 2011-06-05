#include <gtk/gtk.h>

#define S(WIDGET, EVENT, CALLBACK, DATA)\
	g_signal_connect(G_OBJECT(WIDGET),\
						EVENT,\
						G_CALLBACK(CALLBACK),\
						DATA);
#define C(NAME)	void NAME(GtkWidget * widget, gpointer data)

C(on_button_clicked)
{
	gtk_main_quit();
}

int main(int argc, char * argv[])
{
	GtkWidget * window;
	GtkWidget * button;

	// Init gtk
	gtk_init(&argc, &argv);

	// Create window - main
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window),
						"Hello, GTK+!");
	gtk_window_set_default_size(GTK_WINDOW(window),
								200, 50);
	S(window, "destroy", gtk_main_quit, NULL);

	// Create button - exit
	button = gtk_button_new_with_label("exit");
	gtk_container_add(GTK_CONTAINER(window),
						button);
	S(button, "clicked", on_button_clicked, NULL);


	// Show window
	/*
	gtk_widget_show(button);
	gtk_widget_show(window);
	*/
	gtk_widget_show_all(window);

	// Main loop
	gtk_main();

	return 0;
}

