#if 0
# compile by:		bash bpm_counter.c
# run     by:		bash bpm_counter.c run
# clean   by:		bash bpm_counter.c clean
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

// support for lambda function
#define $(ret,def)		({ret $$ def &$$;})


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Elementary.h>
#include <jack/jack.h>




static double volumn;
static jack_port_t * port_out;
static Evas_Object * slider;




EAPI_MAIN int elm_main(int argc, char * argv[])
{
	srand(time(NULL));

	// init jack
	jack_client_t * cli;

	if (!(cli = jack_client_open("BPM Counter", 0, NULL))) {
		fprintf(stderr, "Oh, where are you, my dear Jack?\n");
		return 1;
	}

	jack_set_process_callback(cli,
		$(int, (jack_nframes_t nframes, void * arg) {
			float * out = jack_port_get_buffer(port_out, nframes);

			jack_nframes_t i;
			for(i=0; i<nframes; i++) {
				double noise = rand() & 0xFFFF;
				noise /= (double)0xFFFF;
				noise = noise * 2 - 1;
				noise *= volumn / 100.0;
				out[i] = noise;
			}

			return 0;
		}), NULL);

	port_out = jack_port_register(cli, "metro",
			JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);

	if (jack_activate(cli)) {
		fprintf(stderr, "Oh my dear Jack, are you alive?");
		return 1;
	}


	// init GUI
	Evas_Object * win;
	win = elm_win_add(NULL, "bpm_counter", ELM_WIN_BASIC);
	elm_win_title_set(win, "BPM Counter");
	evas_object_smart_callback_add(win,
			"delete,request", (void *)&elm_exit, NULL);

	Evas_Object * bg;
	bg = elm_bg_add(win);
	evas_object_size_hint_weight_set(bg, 1.0, 1.0);
	elm_win_resize_object_add(win, bg);
	evas_object_show(bg);

	slider = elm_slider_add(win);
	elm_object_text_set(slider, "volumn:");
	elm_slider_unit_format_set(slider, "%1.1f%%");
	elm_slider_min_max_set(slider, 0, 100);
	evas_object_size_hint_weight_set(slider, 1.0, 1.0);
	elm_win_resize_object_add(win, slider);
	evas_object_smart_callback_add(slider, "changed",
		$(void, (void * data, Evas_Object * o, void * ev) {
			volumn = elm_slider_value_get(o);
		}), NULL);
	evas_object_show(slider);

	evas_object_show(win);

	elm_run();
	elm_shutdown();
	jack_client_close(cli);

	return 0;
}
ELM_MAIN()

