
#include <linux/joystick.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

static Display * display;
static Window    root;

static void get_cursor_pos(int * x, int * y)
{
    int tmp;
	unsigned int tmp2;
    Window fromroot, tmpwin;
    XQueryPointer(display, root, &fromroot, &tmpwin,
							x, y, &tmp, &tmp, &tmp2);
}

static void set_cursor_pos(int x, int y)
{
    XWarpPointer(display, None, root, 0, 0, 0, 0, x, y);
    XFlush(display);
}

static void mouse_down(int button)
{
    XEvent event;
    memset(&event, 0, sizeof(event));
 
    event.type = ButtonPress;
    event.xbutton.button = button;
    event.xbutton.same_screen = True;
 
    XQueryPointer(display, RootWindow(display, DefaultScreen(display)),
				&event.xbutton.root, &event.xbutton.window,
				&event.xbutton.x_root, &event.xbutton.y_root,
				&event.xbutton.x, &event.xbutton.y, &event.xbutton.state);
 
    event.xbutton.subwindow = event.xbutton.window;
 
    while (event.xbutton.subwindow) {
		event.xbutton.window = event.xbutton.subwindow;

		XQueryPointer(display, event.xbutton.window, &event.xbutton.root,
					&event.xbutton.subwindow, &event.xbutton.x_root,
					&event.xbutton.y_root, &event.xbutton.x,
					&event.xbutton.y, &event.xbutton.state);
	}
 
    if (!XSendEvent(display, PointerWindow, True, 0xfff, &event))
		fprintf(stderr, "Failed0.\n");
 
    XFlush(display);
} 

static void mouse_up(int button)
{
    XEvent event;
    memset(&event, 0, sizeof(event));
 
    event.type = ButtonRelease;
    event.xbutton.state = 0x100;
    event.xbutton.same_screen = True;
 
    XQueryPointer(display, RootWindow(display, DefaultScreen(display)),
				&event.xbutton.root, &event.xbutton.window,
				&event.xbutton.x_root, &event.xbutton.y_root,
				&event.xbutton.x, &event.xbutton.y, &event.xbutton.state);
 
    event.xbutton.subwindow = event.xbutton.window;
 
    while (event.xbutton.subwindow) {
		event.xbutton.window = event.xbutton.subwindow;

		XQueryPointer(display, event.xbutton.window, &event.xbutton.root,
					&event.xbutton.subwindow, &event.xbutton.x_root,
					&event.xbutton.y_root, &event.xbutton.x,
					&event.xbutton.y, &event.xbutton.state);
	}

    if (!XSendEvent(display, PointerWindow, True, 0xfff, &event))
		fprintf(stderr, "Failed1!\n");
 
    XFlush(display);
}

int main(int argc,char * argv[])
{
	int fd;
	int getnum;
	int success;
	struct js_event e;

	fd = open(argv[1], O_RDONLY);
	if (fd == -1) {
		fprintf(stderr, "error: joystick open failed!\n");
		return 1;
	}

    if (!(display = XOpenDisplay(NULL))) {
        fprintf(stderr, "Cannot open local X-display.\n");
        return;
    }
    root = DefaultRootWindow(display);

	set_cursor_pos(0, 0);

	while (1) {
		read(fd, &e, sizeof(struct js_event));
		printf("value:%d type:%d number:%d\n", e.value, e.type, e.number);
		if (e.type == 2) {
			if (e.number == 0) {	// Left-Right
				if (e.value > 0) {
					int x, y;
					get_cursor_pos(&x, &y);
					x += 10;
					set_cursor_pos(x, y);
				}
				else if (e.value < 0){
				}
			}
		}
	}

	return 0;
}

