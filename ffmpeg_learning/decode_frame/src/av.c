#include "av.h"
#include "util.h"
#include <assert.h>

#define ANY		(void *)

void AV_init()
{
	av_register_all();
}

AV * AV_open(const char * file)
{
	AV * av = calloc(sizeof(AV), 1);

	assert(avformat_open_input(&av->fmt_ctx, file, NULL, NULL) == 0);
	assert(avformat_find_stream_info(av->fmt_ctx, NULL) >= 0);

	// dump information to stderr
	av_dump_format(av->fmt_ctx, 0, file, 0);

	// find the first video stream
	int i;
	for (i=0; i<av->fmt_ctx->nb_streams; i++)
		if (av->fmt_ctx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
			break;
	assert(i < av->fmt_ctx->nb_streams);
	av->video_stream_id = i;

	av->codec_ctx = av->fmt_ctx->streams[i]->codec;
	av->codec = avcodec_find_decoder(av->codec_ctx->codec_id);
	assert(av->codec);

	AVDictionary * options_dict = NULL;
	assert(avcodec_open2(av->codec_ctx, av->codec, &options_dict) >= 0);

	#define sws_get_context(args...) sws_getContext(args);
	av->sws_ctx = sws_get_context(av->codec_ctx->width,
			av->codec_ctx->height, av->codec_ctx->pix_fmt,
			av->codec_ctx->width, av->codec_ctx->height,
			PIX_FMT_RGB24, SWS_BILINEAR, NULL, NULL, NULL);

	av->frame = avcodec_alloc_frame();
	av->rgb   = avcodec_alloc_frame();
	size_t nb = avpicture_get_size(PIX_FMT_RGB24,
			av->codec_ctx->width, av->codec_ctx->height);
	av->buffer= av_malloc(nb * sizeof(uint8_t));
	avpicture_fill(ANY av->rgb, av->buffer, PIX_FMT_RGB24,
			av->codec_ctx->width, av->codec_ctx->height);

	av->f = malloc(sizeof(AV_Frame));
	av->f->w = av->codec_ctx->width;
	av->f->h = av->codec_ctx->height;
	av->f->argb = malloc(sizeof(AV_ARGB) * av->f->w * av->f->h);

	return av;
}

void AV_close(AV * av)
{
	free(av->f->argb);
	free(av->f);
	av_free(av->buffer);
	av_free(av->rgb);
	av_free(av->frame);
	avcodec_close(av->codec_ctx);
	avformat_close_input(&av->fmt_ctx);
	free(av);
}

AV_Frame * AV_read_frame(AV * av)
{
	AVPacket packet;
	int frame_done;

	while (av_read_frame(av->fmt_ctx, &packet) >= 0) {
		if (packet.stream_index == av->video_stream_id) {
			avcodec_decode_video2(av->codec_ctx, av->frame, &frame_done, &packet);
			if (frame_done) {
				// convert the image from its native format to RGB
				sws_scale(av->sws_ctx,
						(uint8_t const * const *)av->frame->data,
						av->frame->linesize, 0, av->codec_ctx->height,
						av->rgb->data, av->rgb->linesize);

				// convert RGB to ARGB.

				AV_ARGB * argb = av->f->argb;
				for (int i=0; i<av->f->w*av->f->h; i++) {
					argb[i].a = 255;
					argb[i].r = av->rgb->data[0][i*3 + 0];
					argb[i].g = av->rgb->data[0][i*3 + 1];
					argb[i].b = av->rgb->data[0][i*3 + 2];
				}
//				SaveFrame(pFrameRGB, pCodecCtx->width, pCodecCtx->height, 
//						i);
				av_free_packet(&packet);
				return av->f;
			}
		}
		av_free_packet(&packet);
	}

	return NULL;
}

