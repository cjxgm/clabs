#include <gtk/gtk.h>
#include <assert.h>
#include <stdlib.h>

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
 * Types
 * 		You know what's this for.
 */

/* node_t is used for the storage of button and command to be executed
 * in the Drawer.
 */
typedef struct _node {
	GtkWidget * button;
	gchar	  * cmd;
} node_t;

/***********************************************************************
 * Global variables
 * 		You know what's this for.
 */
GtkWidget * window_main;
GtkWidget * hbox;
GtkWidget * vbox;
GtkWidget * button_about;
GtkWidget * button_exit;
node_t	  * nodes[512] = { NULL };
node_t	 ** unused_node = nodes;

/***********************************************************************
 * PROTOTYPE
 * 		You know what's this for.
 */
C(on_button_about_clicked);
C(on_button_exit_clicked);
C(on_button_clicked);

void load_config(void);
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
	gtk_window_set_title(GTK_WINDOW(window_main), "Drawer");
	gtk_window_set_default_size(GTK_WINDOW(window_main), 200, 30);
	gtk_container_set_border_width(GTK_CONTAINER(window_main), 6);
	S(window_main, "destroy", gtk_main_quit, NULL);

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
	gtk_box_pack_start(GTK_BOX(hbox), button_about,	TRUE,	TRUE,	2);
	gtk_box_pack_start(GTK_BOX(hbox), button_exit,	TRUE,	TRUE,	2);

	// Load configs
	load_config();

	// Show window and all the widget inside it.
	gtk_widget_show_all(window_main);

	// Main loop
	gtk_main();

	return 0;
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
							   "Button Drawer");
	gtk_about_dialog_set_version (GTK_ABOUT_DIALOG (dialog), "0.1");
	gtk_about_dialog_set_copyright (GTK_ABOUT_DIALOG (dialog),
					"Copyleft (C) eXerigumo Clanjor, 2010-2011.");
	gtk_about_dialog_set_comments (GTK_ABOUT_DIALOG (dialog),
					"A button drawer for launching programs.");

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

/***********************************************************************
 * on_button_clicked
 * 		When a button in the drawer is clicked, this function will be
 *		called. Parameter <data> is the ptr to that node.
 *		(See Macro <C> and Type <node_t> and Function <load_config>)
 */
C(on_button_clicked)
{
	node_t * node = data;
	int err;
	gchar * err_msg;

	err = system(node->cmd);
	if (err) {
		err_msg = g_strconcat("An error occured!\n"
							"Cannot execute '", node->cmd, "'!", NULL);
		msgbox(NULL, "Error", err_msg);
		g_free(err_msg);
	}
}

/***********************************************************************
 * msgbox
 *		Popup a message box
 */
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

/***********************************************************************
 * load_config
 * 		Load the config file which is called "gtkapp.conf", analysis, and
 * 		generate nodes (with buttons) for the drawer.
 */
void load_config(void)
{
	FILE * fp;
	GtkWidget * button;
	gchar * label_name;
	gchar * tmp;
	char buf[2];
	int ch;

	fp = fopen("gtkapp.conf", "r");
	if (!fp) return;

	buf[1] = 0;
	while (!feof(fp)) {
		label_name = g_strconcat("", NULL);
		while ((ch = getc(fp)) != '\n') {
			if (ch == EOF) {
				g_free(label_name);
				return;
			}

			tmp = label_name;
			buf[0] = ch;
			label_name = g_strconcat(label_name, buf, NULL);
			g_free(tmp);
		}

		button = gtk_button_new_with_label(label_name);

		label_name = g_strconcat("", NULL);
		while ((ch = getc(fp)) != '\n') {
			if (ch == EOF) break;

			tmp = label_name;
			buf[0] = ch;
			label_name = g_strconcat(label_name, buf, NULL);
			g_free(tmp);
		}

		*unused_node = malloc(sizeof(node_t));
		(*unused_node)->cmd = label_name;

		S(button, "clicked", on_button_clicked, *unused_node);
		gtk_box_pack_start(GTK_BOX(vbox), button, TRUE, TRUE, 2);

		(*unused_node)->button = button;
		unused_node++;
	}
}

