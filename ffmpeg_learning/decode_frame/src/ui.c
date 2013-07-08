// vim: noet ts=4 sw=4 sts=0
#include <Elementary.h>
#include <string.h>
#include "ui.h"
#include "util.h"
#include "av.h"




#define ANY		(void *)




<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
static win          :* win;
static object_image :* img;

static AV * av;




EAPI_MAIN int elm_main(int argc, char * argv[]);




void ui_run()
{
	ELM_MAIN()
	main(0, NULL);
}


EAPI_MAIN int elm_main(int argc, char * argv[])
{
	//------------------- main window
	{ __ :+ win(NULL, "ffmpeg_decode_frame", ::BASIC); win = __; }
	win :  title("ffmpeg learning: decode a frame");
	win :  resize(800, 600);
	win :- delete,request(ANY elm_exit, NULL);

	bg  :+ bg(win);
	bg  :  hint_weight(::HINT_EXPAND, ::HINT_EXPAND);
	win :  resize_object(bg);
	bg  :  show;

	//------------------- something
	img =  evas_object_image_add(win::evas);
	img: colorspace(::COLORSPACE_ARGB8888);
	img :  move(0, 30);

	av = AV_open("logo.webm");

	AV_Frame * fr = AV_read_frame(av);
	img: size(fr->w, fr->h);
	img: resize(fr->w, fr->h);
	img: image_fill(0, 0, fr->w, fr->h);
	img: image_data(fr->argb);
	img: show;

	//------------------- button
	btn :+ button(win);
	btn :  text("next frame");
	btn :  move(0, 0);
	btn :  resize(100, 30);
	btn :  show;
	void next_frame()
	{
		AV_Frame * fr = AV_read_frame(av);
		AV_ARGB a = { .a = 255, .r = 255, .g = 255, .b = 255 };
		img: colorspace(::COLORSPACE_ARGB8888);
		img: image_data(fr->argb);
		for (int i=100; i<200; i++)
			fr->argb[i * fr->w + i] = a;
		img: image_data_update_add(0, 0, fr->w, fr->h);
	}
	btn :- clicked(ANY next_frame, NULL);

	//------------------- done!
	win: show;

	elm_run();
	elm_shutdown();
	AV_close(av);

	return 0;
}
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

