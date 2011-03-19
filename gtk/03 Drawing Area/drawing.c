#include <gtk/gtk.h>

GtkWidget *window, *drawing;

void on_drawing_show(GtkWidget *widget, gpointer data)
{
	GdkGC *gc = widget->style->fg_gc[GTK_WIDGET_STATE(widget)];
	GdkDrawable *drawable = widget->window;
	GdkColor color;

	gdk_draw_line(drawable, gc, 10, 10, 100, 100);
}

int main(int argc, char *argv[])
{
	gtk_init(&argc, &argv);

	// === Create Widgets ===
	// window
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "你好, GTK+!");

	g_signal_connect(GTK_OBJECT(window), "destroy",
					G_CALLBACK(gtk_main_quit), NULL);

	// drawing area
	drawing = gtk_drawing_area_new();
	gtk_widget_set_size_request(drawing, 320, 240);
	gtk_container_add(GTK_CONTAINER(window), drawing);

	g_signal_connect(GTK_OBJECT(drawing), "show",
					G_CALLBACK(on_drawing_show), NULL);

	// === Main ===
	gtk_widget_show_all(window);
	gtk_main();

	return 0;
}
