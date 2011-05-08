#include "demolino.h"
#include <math.h>

#define DEMO_TITLE		"Effect - Colorful Clouds"
#define DEMO_WIN_SIZE	640, 480

#ifndef PI
#define PI 3.1415927f
#endif

#ifndef SQR
#define SQR(X) ((X)*(X))
#endif

#ifndef RECT
#define RECT(X, Y, W, H) glRectf(X, Y, (X) + (W), (Y) + (H))
#endif

// How many control points?
#define CPS_SIZE	20

// type: controll point
typedef struct _ctl_point {
	int		x;		// x
	int		y;		// y
	int		dist;	// effect distance
	float	r;		//`.
	float	g;		// | color
	float	b;		///

	// 'above' will become 'below' gradually...
	int		dx;		// destination x
	int		dy;		// destination y
	int		ddist;	// destination effect distance
	float	dr;		//`.
	float	dg;		// | destination color
	float	db;		///
} ctl_point_t;

static int				 frame	= 0;
static ctl_point_t		*cps	= NULL; // Control PointS
static int				 w		= 80;	// the draw width
static int				 h		= 80;	// the draw height

// prototype
void draw(void);

int main(void)
{
	printf("Effect - Colorful Clouds\n");
	printf("    Coded by eXerigumo Clanjor\n");
	printf("    Under GPLv3, ABSOLUTELY NO WARRANTY!!!\n");
	printf("Keys:\n");
	printf("    + =     Higher resolution\n");
	printf("    - _     Lower resolution\n");
	printf("    ESC     Exit\n");
	fflush(stdout);

	demo_main(DEMO_TITLE,		// title
				DEMO_WIN_SIZE,	// w, h
				/** TODO:
				 * Following view_w and view_h look strange,
				 * don't they? I don't know why these values
				 * work so fine.
				 */
				240,			// view_w
				240,			// view_h
				100,			// view_z
				30);			// fps

	return 0;
}

void init(void)
{
	int i;

	glClearColor(0, 0, 0, 0);

	// init control points
	cps = calloc(sizeof(ctl_point_t), CPS_SIZE);
	for (i=0; i<CPS_SIZE; i++) {
		cps[i].x		= rand() % w;
		cps[i].y		= rand() % h;
		cps[i].dist		= rand() % ((w<h ? w : h) / 4);
		cps[i].r		= (rand() % 101) / 100.0;
		cps[i].g		= (rand() % 101) / 100.0;
		cps[i].b		= (rand() % 101) / 100.0;

		cps[i].dx		= rand() % w;
		cps[i].dy		= rand() % h;
		cps[i].ddist	= rand() % ((w<h ? w : h) / 4);
		cps[i].dr		= (rand() % 101) / 100.0;
		cps[i].dg		= (rand() % 101) / 100.0;
		cps[i].db		= (rand() % 101) / 100.0;
	}

}

void timer(int value)
{
#define P(SI, DI, STEP, NEW)\
	if (SI < DI)\
		SI+=STEP;\
	else if (SI > DI)\
		SI-=STEP;\
	else\
		DI = NEW;

#define P_CPS(SI, DI, STEP, NEW)	P(cps[i].SI, cps[i].DI, STEP, NEW)

	int i;

	for (i=0; i<CPS_SIZE; i++) {
		P_CPS(x,	dx,		1,		rand() % w);
		P_CPS(y,	dy,		1,		rand() % h);
		P_CPS(dist,	ddist,	1,		rand() % ((w<h ? w : h) / 4));
		P_CPS(r,	dr,		0.01,	(rand() % 101) / 100.0);
		P_CPS(g,	dg,		0.01,	(rand() % 101) / 100.0);
		P_CPS(b,	db,		0.01,	(rand() % 101) / 100.0);
	}

	frame++;
#undef P
#undef P_CPS
}

void render(void)
{
	CLEAR;

	glColor3f(0.5, 0.25, 0.25);

	PUSH;
	glTranslatef(160, 200, 10);
	glScalef(12, 12, 1);
	draw_logo();
	POP;

	PUSH;
	glTranslatef(-280, 200, 10);
	glScalef(10, 10, 1);
	draw_string("Colorful");
	POP;
	PUSH;
	glTranslatef(-278, 170, 10);
	glScalef(13, 13, 1);
	draw_string("Clouds");
	POP;

	PUSH;
	draw();
	POP;

	SWAP;
}

void draw(void)
{
	float	aw	= 640.0 / w;	// aspect ratio of width
	float	ah	= 480.0 / h;	// aspect ratio of height

	int		i;
	int		x;
	int		y;
	float	r;	//`.
	float	g;	// | color
	float	b;	///

	for (y=0; y<h; y++)
		for (x=0; x<w; x++) {
			r = 1.0f;
			g = 1.0f;
			b = 1.0f;

			for (i=0; i<CPS_SIZE; i++) {
				int dx		= (x-cps[i].x);		// distance of x
				int dy		= (y-cps[i].y);		// distance of y
				int dd		= SQR(dx) + SQR(dy);// squared distance
				int sqrd	= SQR(cps[i].dist);	// squared effect distance
				if (dd <= sqrd) {
					float	rate =  (float)dd / sqrd;
					r = r * rate + cps[i].r * (1-rate);
					g = g * rate + cps[i].g * (1-rate);
					b = b * rate + cps[i].b * (1-rate);
				}
			}

			glColor3f(r, g, b);
			RECT(x * aw - 320, y * ah - 240, aw, ah);
		}
}

void keypress(byte key, int x, int y)
{
	switch (key){
		case 27: // Esc
			exit(0);
		case 43: // +
		case 61: // =
			w += 10;
			h += 10;
			printf("Resolution changed to (%d, %d).\n", w, h);
			fflush(stdout);
			break;
		case 45: // -
		case 95: // _
			w -= 10;
			h -= 10;
			if (w == 0) w = 10;
			if (h == 0) h = 10;
			printf("Resolution changed to (%d, %d).\n", w, h);
			fflush(stdout);
			break;
		default:
			fprintf(stderr, "key: %d\n", key);
	}
}

