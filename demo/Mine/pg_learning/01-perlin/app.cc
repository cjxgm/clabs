
#include "app.h"
#include "drawing.h"
#include <math.h>

void app_init()
{
}

void app_advance_frame()
{
}

static float noise(int x, int y)
{
	x += y*57;
	x = (x<<13) ^ x;
	return (1.0 - ((x*(x*x*15731 + 789221) + 1376312589) & 0x7fffffff)
			/ 1073741824.0);
}

// cosine interpolation
static float coserp(float a, float b, float x)
{
	float f = 0.5f * (1.0f - cos(3.1415927f * x));
	return a*(1-f) + b*f;
}

// linear interpolation
static float lerp(float v, float vf, float vt, float df, float dt)
{
	return (v-vf) / (vt-vf) * (dt-df) + df;
}

static float noise_smooth(int x, int y)
{
	float corners = (noise(x-1, y-1) + noise(x+1, y-1)
			+ noise(x-1, y+1) + noise(x+1, y+1)) / 16.0f;
	float sides = (noise(x-1, y) + noise(x+1, y)
			+ noise(x, y-1) + noise(x, y+1)) / 8.0f;
	float center = noise(x, y) / 4.0f;
	return corners + sides + center;
}

static float noise_interp(float x, float y)
{
	int int_x = x;
	int int_y = y;
	float frac_x = x - int_x;
	float frac_y = y - int_y;

	float v1 = noise_smooth(x  , y  );
	float v2 = noise_smooth(x+1, y  );
	float v3 = noise_smooth(x  , y+1);
	float v4 = noise_smooth(x+1, y+1);

	float i1 = coserp(v1, v2, frac_x);
	float i2 = coserp(v3, v4, frac_x);

	return coserp(i1, i2, frac_y);
}

static float pnoise(float x, float y, float persistence, float noctave)
{
	float total = 0.0f;
	int i;
	for (i=0; i<noctave; i++) {
		float freq = pow(2, i);
		float amp  = pow(persistence, i);
		total += noise_interp(x*freq, y*freq) * amp;
	}
	return total;
}

void app_draw(int w, int h)
{
	float x0 = 20.0f;
	float y0 = 20.0f;
	float x1 = 40.0f;
	float y1 = 40.0f;
	int x, y;
	for (y=0; y<h; y++)
		for (x=0; x<w; x++) {
			float p = pnoise(lerp(x, 0, w, x0, x1),
					lerp(y, 0, h, y0, y1),
					2.2, 3);
			p = lerp(p, 0, 2, 0, 1);
			if (p<0) p = 0;
			else if (p > 1) p = 1;
			setcolor(lerp(p, 0, 1, 0.0f, 0.8f*255),
					 lerp(p, 0, 1, 0.0f, 0.5f*255),
					 lerp(p, 0, 1, 0.0f, 0.2f*255));
			putpixel(x, y);
		}
}

