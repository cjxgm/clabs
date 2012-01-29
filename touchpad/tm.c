
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <X11/X.h>
#include <X11/Xlib.h>




typedef struct TPInfo
{
	unsigned long time;
	signed short y;
	unsigned short op;
}
TPInfo;


static Display * display;
static Window    root;




float map(float v, float vf, float vt, float df, float dt)
{
	return (v-vf) / (vt-vf) * (dt-df) + df;
}



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


int mouse_move(int dx, int dy)
{
	int x, y;
	get_cursor_pos(&x, &y);
	x += dx*2;
	y += dy*2;
	set_cursor_pos(x, y);
}

static void mouse_click(int button)
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
 
    usleep(100000);

    event.type = ButtonRelease;
    event.xbutton.state = 0x100;
 
    if (!XSendEvent(display, PointerWindow, True, 0xfff, &event))
		fprintf(stderr, "Failed1!\n");
 
    XFlush(display);
}


int main(int argc, char * argv[])
{
	FILE * fp = fopen(argv[1], "r");

    if (!(display = XOpenDisplay(NULL))) {
        fprintf(stderr, "Cannot open local X-display.\n");
        return;
    }
    root = DefaultRootWindow(display);

	TPInfo tpi;
	int x = 0;
	int y = 0;
	int last_x = 0;
	int last_y = 0;
	int moving = 0;
	int dr = 0;
	long tbegin = 0;
	int ilmb = 1;	// ignore LMB

	while (1) {
		if (!fread(&tpi, sizeof(tpi), 1, fp)) {
			fclose(fp);
			fp = fopen(argv[1], "r");
			fread(&tpi, sizeof(tpi), 1, fp);
		}
		/*printf("%u\t  %6hd\t(%hx)\n", tpi.time,
					tpi.y, tpi.op);*/
		if (tpi.op == 0x0002) {	// x
				last_x = x;
				x = map(tpi.y, -32767, 5800, 0, 255);
		}
		else if (tpi.op == 0x0102) { // y
				last_y = y;
				y = map(tpi.y, -32767, 16000, 0, 255);
		}
		else if (tpi.op == 0x0301) { // up
			int t = time(NULL) - tbegin;
			if (!moving) {
				if (t) {
	//				printf("!!! > RMB\n");
					mouse_click(Button3);
					ilmb = 1;
				}
				else {
					if (ilmb) ilmb = 0;
					else {
	//					printf("!!! < LMB\n");
						mouse_click(Button1);
						ilmb = 1;
					}
				}
			}
			else ilmb = 1;
			moving = 0;
		}
		/*printf(">>> %c %4d %4d <--- %4d %4d\n",
				(moving ? 'M' : ' '), x, y, last_x, last_y);*/
		int a = abs(x-last_x) + abs(y-last_y);
		if (a > 10) {
			if (!moving) {
				dr = 0;
				if (x && y)
					tbegin = time(NULL);
			}
		}
		else {
			dr += a;
			if (dr > 10 && moving == 0) moving = 1;
			if (moving) {
				//printf("!!! %+2d %+2d\n", x-last_x, y-last_y);
				mouse_move(x-last_x, y-last_y);
			}
		}
	}

	return 0;
}

