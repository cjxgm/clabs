#include <gtk/gtk.h>

GtkWidget *window, *button;

gboolean on_motion_event(GtkWidget *widget,
				GdkEventMotion *event, gpointer data)
{
	char pos[20];
	sprintf(pos, "(%d, %d)", (int)event->x, (int)event->y);
	gtk_window_set_title(GTK_WINDOW(widget), pos);

	if ((int)event->x < 10 && (int)event->y < 10)
		gtk_widget_show(button);
	else
		gtk_widget_hide(button);

	return FALSE;
}

void on_button_clicked(GtkWidget *widget, gpointer data)
{
	// g_signal_emit_by_name(window, "destroy"); // Why it will go wrong?
	gtk_main_quit();
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

	gtk_widget_add_events(window, GDK_POINTER_MOTION_MASK);
	g_signal_connect(GTK_OBJECT(window), "motion_notify_event",
					G_CALLBACK(on_motion_event), NULL);

	// button
	button = gtk_button_new_with_label("退出");
	gtk_container_add(GTK_CONTAINER(window), button);

	g_signal_connect(GTK_OBJECT(button), "clicked",
					G_CALLBACK(on_button_clicked), NULL);

	// === Main ===
	gtk_widget_show(window);
	gtk_main();

	return 0;
}
