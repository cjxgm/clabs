
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <AL/al.h>
#include <AL/alc.h>

#define error(X, ...) do { \
	/*	fprintf(stderr, "ERROR: " X "\n", __VA_ARGS__); \
		exit(1); */ \
} while (0)

int main()
{
	int err;
	ALCdevice  * dev;
	ALCcontext * ctx;

	if (!(dev = alcOpenDevice(NULL))) error("alcOpenDevice() failed!");
	if (!(ctx = alcCreateContext(dev, NULL))) error("alcCreateContext() failed!");
	alcMakeContextCurrent(ctx);

	ALuint buf;
	alGenBuffers(1, &buf);
	if (alGetError() != AL_NO_ERROR) error("alGenBuffers() failed!");

	ALuint src;
	alGenSources(1, &src);
	if (alGetError() != AL_NO_ERROR) error("alGenSources() failed!");

	alSource3f(src, AL_POSITION,        0.0, 0.0, 0.0);
	alSource3f(src, AL_VELOCITY,        0.0, 0.0, 0.0);
	alSource3f(src, AL_DIRECTION,       0.0, 0.0, 0.0);
	alSourcef (src, AL_ROLLOFF_FACTOR,  0.0          );
	alSourcei (src, AL_SOURCE_RELATIVE, AL_TRUE      );

	ALushort data[8192*2];	// *2 for double channels/stereo
	int i;
	for (i=0; i<sizeof(data)>>2; i++) {
		int t = i%1024;
		//data[(i<<1)] = (t<512 ? t : 512-t)*0xFFFF/512;
		data[(i<<1)] = rand()%0x5FFF;
		data[(i<<1)+1] = (sin(i / 128.0f)/2.0f+0.5f)*0xFFFF;
	}
	alBufferData(buf, AL_FORMAT_STEREO16, data, sizeof(data), 44100);
	alSourceQueueBuffers(src, 1, &buf);

	alSourcePlay(src);

	while (1) {
		while (1) {
			ALenum state;
			alGetSourcei(src, AL_SOURCE_STATE, &state);
			if (state != AL_PLAYING) break;
		}

		alBufferData(buf, AL_FORMAT_STEREO16, data, sizeof(data), 44100);
		printf(":E: alBufferData() --> %X  %d\n", alGetError(), sizeof(data));

		alSourceQueueBuffers(src, 1, &buf);
		printf(":E: alSourceQueueBuffers() --> %X\n", alGetError());

		alSourcePlay(src);
		printf(":E: alSourcePlay() --> %X\n", alGetError());
	}

	return 0;
}

