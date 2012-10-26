
#include <stdio.h>
#include <alsa/asoundlib.h>

#define CHECK(X)	if ((err = (X)) < 0) \
	   printf("error: %s\n", snd_strerror(err));

static snd_seq_t * seq;
static int port;

int main()
{
	int err;

	CHECK(snd_seq_open(&seq, "default", SND_SEQ_OPEN_OUTPUT, 0));
	CHECK(snd_seq_set_client_name(seq, "Cidi Tracker"));
	port = snd_seq_create_simple_port(seq, "Generic",
			SND_SEQ_PORT_CAP_READ | SND_SEQ_PORT_CAP_SUBS_READ,
			SND_SEQ_PORT_TYPE_MIDI_GENERIC);
	CHECK(port);

	// wait here so that user can run aconnect to route it.
	getchar();

	snd_seq_event_t ev;
	snd_seq_ev_clear(&ev);
	snd_seq_ev_set_source(&ev, port);
	snd_seq_ev_set_subs(&ev);
	snd_seq_ev_set_direct(&ev);
	snd_seq_ev_set_note(&ev, 1, 60, 100, 10);
	CHECK(snd_seq_event_output_direct(seq, &ev));

	while (getchar() != 'q');

	return 0;
}

