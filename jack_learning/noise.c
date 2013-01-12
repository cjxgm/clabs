#if 0
# compile by:		bash noise.c
# run     by:		bash noise.c run
# clean   by:		bash noise.c clean
PKGS=""
PKGS="$PKGS $(pkg-config --cflags --libs elementary)"
PKGS="$PKGS $(pkg-config --cflags --libs jack)"
CCFLAGS="-Wall -Werror -Ofast $PKGS"
case $1 in
	clean)	rm -f "${0%.c}"								;;
	run)	[ -x "${0%.c}" ] || bash $0 ; "./${0%.c}"	;;
	*)		gcc $CCFLAGS -o ${0%.c} $0					;;
esac
exit
#endif

// vim: noet ts=4 sw=4 sts=0




#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Elementary.h>
#include <jack/jack.h>




static void end();
static void _change_cb(void * data, Evas_Object * o, void * ev);
static int _process_cb(jack_nframes_t nframes, void * arg);

static double volumn;
static jack_port_t * port_in;
static jack_port_t * port_out;




EAPI_MAIN int elm_main(int argc, char * argv[])
{
	srand(time(NULL));

	// init jack
	jack_client_t * cli;

	if (!(cli = jack_client_open("Noise", 0, NULL))) {
		fprintf (stderr, "Oh, where are you, my dear Jack?\n");
		return 1;
	}

	jack_set_process_callback(cli, _process_cb, NULL);
	jack_on_shutdown(cli, (void *)&end, NULL);

	port_in = jack_port_register(cli, "input",
			JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);
	port_out = jack_port_register(cli, "output",
			JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);

	if (jack_activate (cli)) {
		fprintf (stderr, "Oh my dear Jack, are you alive?");
		return 1;
	}


	// init GUI
	Evas_Object * win;
	win = elm_win_add(NULL, "noise", ELM_WIN_BASIC);
	elm_win_title_set(win, "Jack Noise Generator");
	evas_object_smart_callback_add(win,
			"delete,request", (void *)&end, NULL);

	Evas_Object * bg;
	bg = elm_bg_add(win);
	evas_object_size_hint_weight_set(bg, 1.0, 1.0);
	elm_win_resize_object_add(win, bg);
	evas_object_show(bg);

	Evas_Object * slider;
	slider = elm_slider_add(win);
	elm_object_text_set(slider, "volumn:");
	elm_slider_unit_format_set(slider, "%1.1f%%");
	elm_slider_min_max_set(slider, 0, 100);
	evas_object_size_hint_weight_set(slider, 1.0, 1.0);
	elm_win_resize_object_add(win, slider);
	evas_object_smart_callback_add(slider, "changed", &_change_cb, NULL);
	evas_object_show(slider);

	evas_object_show(win);

	elm_run();
	elm_shutdown();
	jack_client_close(cli);

	return 0;
}
ELM_MAIN()


static void end()
{
	elm_exit();
}


static void _change_cb(void * data, Evas_Object * o, void * ev)
{
	volumn = elm_slider_value_get(o);
}


static int _process_cb(jack_nframes_t nframes, void * arg)
{
	float * in  = jack_port_get_buffer(port_in , nframes);
	float * out = jack_port_get_buffer(port_out, nframes);

	jack_nframes_t i;
	for(i=0; i<nframes; i++) {
		double noise = rand() & 0xFFFF;
		noise /= (double)0xFFFF;
		noise = noise * 2 - 1;
		noise *= volumn / 100.0;
		out[i] = in[i] + noise;
	}

	return 0;      
}

