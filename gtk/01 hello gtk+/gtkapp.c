#include <gtk/gtk.h>

int main(int argc, char * argv[])
{
	GtkWidget * window;

	// Init gtk
	gtk_init(&argc, &argv);

	// Create main window
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window),
						"Hello, GTK+!");

	// Show window
	gtk_widget_show(window);

	// Main loop
	gtk_main();

	return 0;
}

