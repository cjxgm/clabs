// vim: noet ts=4 sw=4 sts=0
// decoder
#ifndef __AV__
#define __AV__

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>


typedef struct AV_ARGB
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
	
	int video_stream_id;
}
AV;

void AV_init();
AV * AV_open(const char * file);
void AV_close(AV * av);
AV_Frame * AV_read_frame(AV * av);


#endif

