#include <gtk/gtk.h>

/***********************************************************************
 * MACRO DEFINITION
 * 		Some useful macro will be defined here.
 */
// Signal Connecting Macro
#define S(WIDGET, EVENT, CALLBACK, DATA)\
	g_signal_connect(G_OBJECT(WIDGET), EVENT,\
						G_CALLBACK(CALLBACK), DATA);

// Callback Function Declaring Macro
#define C(NAME)	void NAME(GtkWidget * widget, gpointer data)

/***********************************************************************
 * PROTOTYPE
 * 		You know what's this for.
 */
C(on_button_hello_clicked);
C(on_button_exit_clicked);

/***********************************************************************
 * main
 * 		The main routine.
 *		    I may write some comments like
 *		'Create <object> - <name> -> <parent>',
 *		which means I will create a <object> called <name>,
 *		and then I will put it inside <parent>.
 *		You should know it when you see them.
 */
int main(int argc, char * argv[])
{
	GtkWidget * window_main;
	GtkWidget * hbox;
	GtkWidget * vbox;
	GtkWidget * button_hello;
	GtkWidget * button_exit;
	GtkWidget * editor;

	// Init gtk
	gtk_init(&argc, &argv);

	// Create window - window_main
	window_main = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window_main), "Hello, GTK+!");
	gtk_window_set_default_size(GTK_WINDOW(window_main), 200, 30);
	gtk_container_set_border_width(GTK_CONTAINER(window_main), 6);
	S(window_main, "destroy", gtk_main_quit, NULL);

	// Create button - button_exit -> hbox
	button_exit = gtk_button_new_with_label("exit");
	S(button_exit, "clicked", on_button_exit_clicked, NULL);

	// Create button - button_hello -> hbox
	button_hello = gtk_button_new_with_label("hello");
	S(button_hello, "clicked", on_button_hello_clicked, NULL);

	// Create entry - editor -> hbox
	editor = gtk_entry_new();

	// Create vbox - vbox -> window_main
	vbox = gtk_vbox_new(FALSE, 0);
	gtk_container_add(GTK_CONTAINER(window_main), vbox);

	// Create hbox - hbox -> vbox
	hbox = gtk_hbox_new(FALSE, 0);
/*	gtk_box_pack_start(GTK_BOX(box ), child, 	expand, fill, padding) */
	gtk_box_pack_start(GTK_BOX(vbox), hbox,			TRUE,	TRUE,	2);
	gtk_box_pack_start(GTK_BOX(hbox), editor, 		TRUE,	TRUE,	2);
	gtk_box_pack_start(GTK_BOX(hbox), button_hello,	TRUE,	TRUE,	2);
	gtk_box_pack_start(GTK_BOX(hbox), button_exit,	FALSE,	FALSE,	2);

	// Show window and all the widget inside it.
	gtk_widget_show_all(window_main);

	// Main loop
	gtk_main();

	return 0;
}

/***********************************************************************
 * on_button_hello_clicked
 * 		Process events when <button_hello> is clicked.
 */
C(on_button_hello_clicked)
{
	g_print("Oh, you clicked me ...\n");
}

/***********************************************************************
 * on_button_exit_clicked
 * 		Process events when <button_exit> is clicked.
 */
C(on_button_exit_clicked)
{
	gtk_main_quit();
}

