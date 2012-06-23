
/************************************************************
 * cp: render_score.c
 *
 * Licensed under GPLv2.
 * ABSOLUTELY NO WARRANTY!
 *
 * Copyright (C) eXerigumo Clanjor (哆啦比猫/兰威举).
 ************************************************************/

#include "renders.h"
#include "video.h"
#include "font.h"
#include "../core/core.h"
#include "../audio/music.h"
#include "../audio/seq.h"
#include "../audio/audio.h"
#include <stdio.h>

static void draw_scope(float w, float h);




void render_score()
{
	video_view2d(0, 0, video_w, video_h);
	draw_scope(video_w, video_h);
}




static void draw_scope(float w, float h)
{
	static const char * note_names[] = {
		"C-", "C#",
		"D-", "D#",
		"E-",
		"F-", "F#",
		"G-", "G#",
		"A-", "A#",
		"B-",
	};
	char buf[64];

	// draw scores
	float y = 0.0f;
	y = h/2.0f + ticks() / music_mspr * 25;

	glColor3f(0.16f, 0.16f, 0.16f);
	glRectf(0, 0, w, h);

	float nw = font_width("... ..") + 16.0f;	// note string style
	int i, j;
	for (j=0; j<music_nscore; j++) {
		u16 note;
		for (i=0; (note=music_scores[j][i]) != 0xFFFF; i++) {
			// convert note to string
			if (!note) sprintf(buf, "... %2.2X", note&0xFF);
			else if (!(note>>12)) sprintf(buf, "=== %2.2X", note&0xFF);
			else sprintf(buf, "%s%d %2.2X", note_names[(note>>8)&0x0F],
					note>>12, note&0xFF);

			// current playing
			if (i == seq_get_all()[j].notepos) {
				if (note) glColor3f(0.8f, 0.5f, 0.2f);
				else glColor3f(0.4f, 0.2f, 0.1f);
				glRectf(j*nw, y-25 - i*25, (j+1)*nw, y - i*25);
			}
			else {
				glColor3f(0.2f, 0.2f, 0.2f);
				if (i & 1) glRectf(j*nw, y-25 - i*25, (j+1)*nw, y - i*25);
			}

			// draw the note string
			if (note) glColor3f(1.0f, 1.0f, 1.0f);
			else glColor3f(0.6f, 0.6f, 0.6f);
			font_render(buf, j*nw + 8.0f, y-25 - i*25 + 3);
		}
	}

	// draw time indicator
	glColor3f(0.5f, 1.0f, 0.2f);
	sprintf(buf, "%2.2f", ticks()/1000.0f);
	float tw = font_width(buf)+12.0f;
	glRectf(w-tw, h/2.0f - 25.0f, w, h/2.0f);
	glColor3f(0.2f, 0.5f, 1.0f);
	font_render(buf, w-tw + 6.0f, h/2.0f - 22.0f);

	glColor3f(0.2f, 0.5f, 0.1f);
	glBegin(GL_LINE_STRIP);
	for (i=0; i<AUDIO_BUF_SIZE; i++)
		glVertex2f((float)i/AUDIO_BUF_SIZE*w,
				lerp(audio_get_buf()[i], 0, 0xFF,
						h/2.0f - 100.0f, h/2.0f + 100.0f));
	glEnd();
	glColor3f(1.0f, 1.0f, 1.0f);
	glPointSize(2.0);
	glBegin(GL_POINTS);
	for (i=0; i<AUDIO_BUF_SIZE; i+=8)
		glVertex2f((float)i/AUDIO_BUF_SIZE*w,
				lerp(audio_get_buf()[i], 0, 0xFF,
						h/2.0f - 100.0f, h/2.0f + 100.0f));
	glEnd();
	glPointSize(1.0);

	glLineWidth(2);
	glColor3f(0.5f, 1.0f, 0.2f);
	glBegin(GL_LINE_STRIP);
	glVertex2f(0.0f, h / 2.0f);
	glVertex2f(w, h / 2.0f);
	glEnd();
	glLineWidth(1);
}

