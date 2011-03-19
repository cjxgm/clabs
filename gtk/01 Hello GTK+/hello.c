#include <gtk/gtk.h>

void on_button_clicked(GtkWidget *button, gpointer data)
{
	g_print("好吧，%s\n", (char*)data);
	gtk_widget_destroy(button);
}

int main(int argc, char *argv[])
{
	gtk_init(&argc, &argv);

	// === Create Widgets ===
	// window
	GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "你好, GTK+!");

	g_signal_connect(GTK_OBJECT(window), "destroy",
					G_CALLBACK(gtk_main_quit), NULL);

	// button
	GtkWidget *button = gtk_button_new_with_label("有种点我啊！");
	gtk_container_add(GTK_CONTAINER(window), button);

	g_signal_connect(GTK_OBJECT(button), "clicked",
					G_CALLBACK(on_button_clicked), "算你狠！");

	// === Main ===
	gtk_widget_show_all(window);
	gtk_main();

	return 0;
}
