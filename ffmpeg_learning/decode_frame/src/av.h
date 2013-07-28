// vim: noet ts=4 sw=4 sts=0
// decoder
#pragma once

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include "util.h"


typedef union AV_ARGB
{
	unsigned long argb;
	struct {
		unsigned char b;
		unsigned char g;
		unsigned char r;
		unsigned char a;
	};
}
AV_ARGB;

typedef struct AV_Frame
{
	size_t w;
	size_t h;
	AV_ARGB * argb;
}
AV_Frame;

typedef struct AV
{
	struct SwsContext * sws_ctx;
	AVFormatContext   * fmt_ctx;
	AVCodecContext    * codec_ctx;
	AVCodec           * codec;
	AVFrame           * frame;
	AVFrame           * rgb;
	unsigned char     * buffer;
	AV_Frame          * f;

	size_t frame_id;
	int video_stream_id;
}
AV;

void AV_init() CONSTRUCTOR;

AV * AV_open(const char * file);
void AV_close(AV * av);

void AV_read_frame(AV * av);
static inline AV_Frame * AV_get_frame(AV * av) { return av->f;        }
static inline size_t  AV_get_frame_id(AV * av) { return av->frame_id; }

void AV_seek(AV * av, size_t frame);

