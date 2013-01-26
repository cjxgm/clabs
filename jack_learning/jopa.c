#if 0

# Jack over PulseAudio
# usage:
#		jackd -d dummy &
#		bash jopa.c run
# 	then connect jack output ports to jopa.

# compile by:		bash jopa.c
# run     by:		bash jopa.c run
# clean   by:		bash jopa.c clean
PKGS="-lpulse-simple"
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
#include <unistd.h>
#include <jack/jack.h>
#include <pulse/simple.h>




static jack_port_t * ports[2];	// stereo

static pa_simple * s = NULL;
static pa_sample_spec ss = {
	.format		= PA_SAMPLE_FLOAT32,
	.rate		= 48000,
	.channels	= 2,			// stereo
};




int main()
{
	// init pulse
	s = pa_simple_new(NULL, "Jack over PulseAudio",
			PA_STREAM_PLAYBACK, NULL, "jopa",
			&ss, NULL, NULL, NULL);

	if (!s) {
		fprintf(stderr, "Cannot open pulse audio.\n");
		return 1;
	}
 
	


	// init jack
	jack_client_t * cli;

	if (!(cli = jack_client_open("Jack over PulseAudio", 0, NULL))) {
		fprintf(stderr, "Oh, where are you, my dear Jack?\n");
		return 1;
	}

	jack_set_process_callback(cli,
		$(int, (jack_nframes_t nframes, void * arg) {
			float * L = jack_port_get_buffer(ports[0], nframes);
			float * R = jack_port_get_buffer(ports[1], nframes);
			float stereo[nframes << 1];

			jack_nframes_t i;
			for(i=0; i<nframes; i++) {
				stereo[i<<1] = L[i];
				stereo[(i<<1)|1] = R[i];
			}

			pa_simple_write(s, stereo, sizeof(stereo), NULL);

			return 0;
		}), NULL);

	jack_on_shutdown(cli, $(void, () {
		pa_simple_free(s);
		jack_client_close(cli);
		exit(0);
	}), NULL);

	ports[0] = jack_port_register(cli, "left",
			JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);
	ports[1] = jack_port_register(cli, "right",
			JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);

	if (jack_activate(cli)) {
		fprintf(stderr, "Oh my dear Jack, are you alive?");
		return 1;
	}

//FIXME: not working as desired (connected but no audio signal in)
//	// connect physical outputs to me
//	const char ** phys_ports = jack_get_ports(cli, NULL, NULL,
//			JackPortIsOutput | JackPortIsPhysical);
//	int i;
//	for (i=0; i<2 && phys_ports[i]; i++)
//		jack_connect(cli, phys_ports[i], jack_port_name(ports[i]));

	// don't exit
	while (1) sleep(1);
	return 0;
}

