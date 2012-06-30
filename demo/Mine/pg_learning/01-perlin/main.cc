
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include "config.h"
#include "app.h"

////////////////////////////////////////////////////////////
//
// App Window
//

class AppWin : public Fl_Window
{
	float secs_per_frame;
	float dist;

	void draw();
	static void timer_cb(void * ud);
public:
	AppWin(int w, int h,
				float secs_per_frame);
};

AppWin::AppWin(int w, int h,
				float secs_per_frame)
	: Fl_Window(w, h)
	, dist(60)
{
	app_init();
	this->secs_per_frame = secs_per_frame;
	Fl::add_timeout(secs_per_frame,
					timer_cb,
					(void *)this);
}

void AppWin::draw()
{
	app_draw(w(), h());
}

void AppWin::timer_cb(void * ud)
{
	AppWin * o = (AppWin *)ud;
	o->redraw();
	app_advance_frame();
	Fl::repeat_timeout(o->secs_per_frame, timer_cb, ud);
}

////////////////////////////////////////////////////////////
//
// main
//

int main(int argc, char * argv[])
{
	AppWin * win = new AppWin(PROJECT_WIN_W, PROJECT_WIN_H, PROJECT_SPF);
	win->label(PROJECT_NAME);
	win->end();
	win->show(argc, argv);
	return Fl::run();
}

