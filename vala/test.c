#include <gtk/gtk.h>
#include <glade/glade.h>

int main(int argc, char *argv[]) {
	GladeXML *xml;

	gtk_init(&argc, &argv);

	/* load the interface */
	xml = glade_xml_new("test.glade", NULL, NULL);

	/* connect the signals in the interface */
	glade_xml_signal_autoconnect(xml);

	gtk_widget_show_all(glade_xml_get_widget(xml, "main"));

	/* start the event loop */
	gtk_main();

	return 0;
}
