#if 0
CCFLAGS="-Ilibcrude -Llibcrude -lcrude `(pkg-config --cflags --libs elementary)`"
cpp $CCFLAGS -dD $0 | sed '/__STDC/d' |
	./elm_ui |
	cpp $CCFLAGS | gcc -o /tmp/tmp_bin -x cpp-output - $CCFLAGS
/tmp/tmp_bin
exit 0
#endif
#include <Elementary.h>
#include "util.h"

// compile and run: ./test_elm.pl grid_test.c

// :	object set
// ::	object get
// :+	object add
// :-	smart event bind
// :=	object type cast
// :*	object declare

#define ANY		(void *)
<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
EAPI_MAIN int elm_main(int argc, char * argv[])
{
	win:+win				("test-elm", "test-elm");
	win:resize				(800, 600);
	win:-delete,request		(ANY elm_exit, NULL);

	ctn:+table				(win);	// ctn = container
	ctn:weight				(EXPAND, EXPAND);
	win:resize_object		(ctn);
	ctn:show;

	grid:+grid				(win);
	grid:weight				(EXPAND, EXPAND);
	grid:align				(FILL, FILL);
	ctn:pack				(grid, 0, 0, 1, 1);
	grid:show;

	btn:+button				(win);
	btn:weight				(EXPAND, EXPAND);
	btn:align				(FILL, FILL);
	grid:pack				(btn, 10, 10, 80, 80);
	btn:show;

	slh:+slider				(win);
	slh:min_max				(100, 200);
	slh:weight				(EXPAND, 0);
	slh:align				(FILL, FILL);
	ctn:pack				(slh, 0, 1, 1, 1);
	slh:show;

	slv:+slider				(win);
	slv:min_max				(100, 200);
	slv:horizontal			(false);
	slv:weight				(0, EXPAND);
	slv:align				(FILL, FILL);
	ctn:pack				(slv, 1, 0, 1, 1);
	slv:show;

	ck:+check				(win);
	ck:align				(FILL, FILL);
	ctn:pack				(ck, 1, 1, 1, 1);
	ck:show;

	static int grid_w = 100;
	static int grid_h = 100;

	void on_slider_change(int * size, slider :* sl)
	{
		if (ck::state) {
			grid_w = grid_h = sl::value;
			slh:value		(grid_w);
			slv:value		(grid_w);
		}
		else *size = sl::value;
		grid:size			(grid_w, grid_h);
	}
	slh:-changed			(ANY on_slider_change, &grid_w);
	slv:-changed			(ANY on_slider_change, &grid_w);

	win:show;
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	elm_run();
	elm_shutdown();
	return 0;
}
ELM_MAIN()
// vim: noet ts=4 sw=4 sts=0 ft=c
