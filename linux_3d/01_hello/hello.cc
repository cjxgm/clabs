#include <stdlib.h>
#include <X11/Intrinsic.h>
#include <X11/Xlib.h>

#define SCREEN_XSIZE 320
#define SCREEN_YSIZE 200

int main()
{
	Display *dpy;
	Visual *vis;
	Window w;

	dpy = XOpenDisplay(NULL);
	vis = DefaultVisual(dpy,0);
	w = XCreateWindow(dpy,
			DefaultRootWindow(dpy),
			100, 100,
			SCREEN_XSIZE, SCREEN_YSIZE,
			0,
			CopyFromParent,
			CopyFromParent,
			vis,
			0, NULL);
	XStoreName(dpy, w, "hello");
	XSelectInput(dpy, w, KeyPressMask);
	XEvent event;
	char ch;
	KeySym keysym;
	XComposeStatus xcompstat;
	while(1) {
		if(XCheckWindowEvent(dpy,w,KeyPressMask,&event)) {
			XLookupString(&event.xkey, &ch, 1, &keysym, &xcompstat);
			switch(ch) {
				case 'q': 
					exit(0);
			}
		}
	}

	return 0;
}

