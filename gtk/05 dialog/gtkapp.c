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
 * Global variables
 * 		You know what's this for.
 */
GtkWidget * window_main;
GtkWidget * hbox;
GtkWidget * vbox;
GtkWidget * button_hello;
GtkWidget * button_about;
GtkWidget * button_exit;
GtkWidget * editor;

/***********************************************************************
 * PROTOTYPE
 * 		You know what's this for.
 */
C(on_button_hello_clicked);
C(on_button_about_clicked);
C(on_button_exit_clicked);

void msgbox(GtkWindow * parent, const char * title, const char * text);

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
	// Init gtk
	gtk_init(&argc, &argv);

	// Create window - window_main
	window_main = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window_main), "Hello, GTK+!");
	gtk_window_set_default_size(GTK_WINDOW(window_main), 200, 30);
	gtk_container_set_border_width(GTK_CONTAINER(window_main), 6);
	S(window_main, "destroy", gtk_main_quit, NULL);

	// Create entry - editor -> hbox
	editor = gtk_entry_new();

	// Create button - button_hello -> hbox
	button_hello = gtk_button_new_with_label("hello");
	S(button_hello, "clicked", on_button_hello_clicked, NULL);

	// Create button - button_about -> hbox
	button_about = gtk_button_new_with_label("about");
	S(button_about, "clicked", on_button_about_clicked, NULL);

	// Create button - button_exit -> hbox
	button_exit = gtk_button_new_with_label("exit");
	S(button_exit, "clicked", on_button_exit_clicked, NULL);

	// Create vbox - vbox -> window_main
	vbox = gtk_vbox_new(FALSE, 0);
	gtk_container_add(GTK_CONTAINER(window_main), vbox);

	// Create hbox - hbox -> vbox
	hbox = gtk_hbox_new(FALSE, 0);
/*	gtk_box_pack_start(GTK_BOX(box ), child, 	expand, fill, padding) */
	gtk_box_pack_start(GTK_BOX(vbox), hbox,			TRUE,	TRUE,	2);
	gtk_box_pack_start(GTK_BOX(hbox), editor, 		TRUE,	TRUE,	2);
	gtk_box_pack_start(GTK_BOX(hbox), button_hello,	TRUE,	TRUE,	2);
	gtk_box_pack_start(GTK_BOX(hbox), button_about,	FALSE,	FALSE,	2);
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
	gchar * text;
	const gchar * name;

	name = gtk_entry_get_text(GTK_ENTRY(editor));
	if (name[0] == 0)
		text = g_strconcat("What's your name, please?", NULL);
	else
		text = g_strconcat("Hello, ", name, "!", NULL);
	msgbox(NULL, "Hello!", text);
	g_free(text);
}

/***********************************************************************
 * on_button_about_clicked
 * 		Process events when <button_about> is clicked.
 */
C(on_button_about_clicked)
{
	GtkWidget *dialog;
	GdkPixbuf *logo;
	GError *error = NULL;

	// Create about dialog - dialog
	dialog = gtk_about_dialog_new();

	// Load logo
	logo = gdk_pixbuf_new_from_file("./logo.jpg", &error);
	if (error == NULL)
		gtk_about_dialog_set_logo(GTK_ABOUT_DIALOG(dialog), logo);
	else {
		if (error->domain == GDK_PIXBUF_ERROR)
			g_print ("GdkPixbufError: %s\n", error->message);
		else if (error->domain == G_FILE_ERROR)
			g_print ("GFileError: %s\n", error->message);
		else
			g_print ("An error in the domain: "
					 "%d has occurred!\n", error->domain);
		g_error_free (error);
	}

	// Configure <dialog>
	gtk_about_dialog_set_name (GTK_ABOUT_DIALOG (dialog),
							   "Dialogs' Demo");
	gtk_about_dialog_set_version (GTK_ABOUT_DIALOG (dialog), "1.0");
	gtk_about_dialog_set_copyright (GTK_ABOUT_DIALOG (dialog),
					"Copyleft (C) eXerigumo Clanjor, 2010-2011.");
	gtk_about_dialog_set_comments (GTK_ABOUT_DIALOG (dialog),
								   "Just a test for dialogs.");

	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
}

/***********************************************************************
 * on_button_exit_clicked
 * 		Process events when <button_exit> is clicked.
 */
C(on_button_exit_clicked)
{
	gtk_main_quit();
}

void msgbox(GtkWindow * parent, const char * title, const char * text)
{
	GtkWidget * dialog;

	// Create message dialog - dialog -> parent
	dialog = gtk_message_dialog_new(parent, GTK_DIALOG_MODAL,
									GTK_MESSAGE_INFO,
									GTK_BUTTONS_OK,
									"%s", text);
	gtk_window_set_title(GTK_WINDOW(dialog), title);

	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
}

