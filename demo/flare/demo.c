#include "demolino.h"
#include <math.h>

#define DEMO_TITLE		"Flare - Colorful Clouds"
#define DEMO_WIN_SIZE	480, 480

#ifndef PI
#define PI 3.1415927f
#endif

#define MAKE_FLARE(P,S,R,G,B)\
	glBegin(GL_TRIANGLE_FAN);\
	glColor4f(1.0,1.0,1.0,brightness*0.5);\
	glVertex2f(sx*P,sy*P);\
	glColor4f(R,G,B,0);\
	for(loop=0.0;loop<6.283185307;loop+=6.283185307/6.0)\
	{\
		glVertex2f(sx*P+sin(loop)*S*brightness,sy*P+cos(loop)*S*brightness);\
	}\
	glEnd();

float x=-4,y=-4,dir=135,pitch=5; // position, direction, and pitch. Where you are, where you're looking.

float sun[] = {10,4,10,1}; // The sun's positon.
float sun_dif[] = {1.0,1.0,1.0,0.5}; // This is the colour of the light.

int main(void)
{
	demo_main(DEMO_TITLE,		// title
				DEMO_WIN_SIZE,	// w, h
				/** TODO:
				 * Following view_w and view_h look strange,
				 * don't they? I don't know why these values
				 * work so fine.
				 */
				480,			// view_w
				480,			// view_h
				100,			// view_z
				30);			// fps
}

void init(void)
{
	glTranslatef(0,0,-5);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glLightfv(GL_LIGHT1,GL_POSITION,sun);
	glLightfv(GL_LIGHT1,GL_DIFFUSE,sun_dif);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHTING);
	glEnable(GL_CULL_FACE);
	glClearColor(0.5,0.6,1,0);
	glEnable(GL_DEPTH_TEST);
	glColorMaterial(GL_FRONT,GL_DIFFUSE); // These two lines are so that I can use the glColor command
	glEnable(GL_COLOR_MATERIAL);          // with the lighting.
}

void render(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45,1,0.01,100);
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	glRotatef(pitch,1,0,0); // Set our position and orientition.
	glRotatef(dir,0,1,0);
	glTranslatef(-x,0,-y);

	glLightfv(GL_LIGHT1,GL_POSITION,sun); // Set up the light position. (This multiplys the position by the model matrix so you need to call it every frame after the view is setup.)

	glColor3f(0,1,0); // Make the geometry green.

#include "scene.h" // I found an easy way to use the output from ASE2GL: Put it in a header and include it.

	GLdouble modelMatrix[16]; // The model matrix.
	GLdouble projMatrix[16];  // The projection matrix.
	GLdouble sx,sy,sz;        // Where on the screen the light is.
	int x,y;                  // Used for going through the depth buffer.
	float brightness=0;       // How bright the light is.
	float depth;              // The depth in the framebuffer.
	GLint viewport[4];        // The viewport.

	glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix); // Load the matricies and viewport.
	glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
	glGetIntegerv(GL_VIEWPORT, viewport);

	gluProject(sun[0],sun[1],sun[2],modelMatrix,projMatrix,viewport,&sx,&sy,&sz); // Find out where the light is on the screen.


	for(x=-4;x<=4;x++) // Go through some of the points near the light.
		for(y=-4;y<=4;y++)
		{
			if(viewport[2]/300.0*x+sx < viewport[2] && viewport[2]/300.0*x+sx >= 0 &&
					viewport[3]/300.0*y+sy < viewport[3] && viewport[3]/300.0*y+sy >= 0 ) // If the point is on the screen
			{
				glReadPixels((int)(viewport[2]/300.0*x+sx),(int)(viewport[3]/300.0*y+sy),1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&depth); // Read the depth from the depth buffer.
				if(depth >= sz) brightness += 1.0/81.0; // If the light is infront of what ever was in that spot, increase the brightness.
			}
		}


	if(sx < viewport[2] && sx >= 0 &&
			sy < viewport[3] && sy >= 0 ) // If the light is on the screen
	{
		glReadPixels((int)(sx),(int)(sy),1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&depth);
		if(depth < sz) // but it is behind something.
			brightness = 0; // The light can't be seen.
	}
	else // If the light isn't on the screen
		brightness = 0; // The light can't be seen.


	sx = sx/(float)viewport[2]*2.0-1.0; // Now that we know the brightness, convert
	sy = sy/(float)viewport[2]*2.0-1.0; // The position to a nember between (-1,-1) and (1,1)

	brightness = (1.2-sqrt(sx*sx+sy*sy))*brightness*0.75; // And adjust the brightness so it is brighter at the centre of the screen.

	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity(); // Forget about the view and projection. We will be drawing in 2D for the flares.
	glDisable(GL_DEPTH_TEST); // Disable depth testing, face culling, and lighting.
	glDisable(GL_CULL_FACE);  // You don't need those in 2D.
	glDisable(GL_LIGHTING);

	glBegin(GL_TRIANGLE_FAN); // Make a steak across the screen where the light is.
	glColor4f(0.9,1.0,0.9,brightness*0.5);
	glVertex2f(sx,sy);
	glColor4f(1.0,1.0,1.0,0);
	glVertex2f(sx+2.5*brightness,sy);
	glVertex2f(sx,sy+0.1*brightness);
	glVertex2f(sx-2.5*brightness,sy);
	glVertex2f(sx,sy-0.1*brightness);
	glVertex2f(sx+2.5*brightness,sy);
	glEnd();

	glColor4f(1,1,1,brightness); // Brighten the screen.
	glBegin(GL_QUADS);                
	glTexCoord2f(0,0);
	glVertex2f(sx-2,sy-2);
	glTexCoord2f(1,0);
	glVertex2f(sx+2,sy-2);
	glTexCoord2f(1,1);
	glVertex2f(sx+2,sy+2);
	glTexCoord2f(0,1);
	glVertex2f(sx-2,sy+2);
	glEnd();

	float loop;
	bool big = true;

	glBegin(GL_TRIANGLE_FAN);
	glColor4f(1.0,1.0,1.0,brightness*0.5);
	glVertex2f(sx,sy);
	glColor4f(1.0,1.0,0.5,0);
	for(loop=0.0;loop<6.283185307;loop+=6.283185307/64.0) // Make a 32 point star (64/2=32). Feel free to change that if you want.
	{
		glVertex2f(sx+sin(loop)*(big?2.0*brightness:1.0*brightness),sy+cos(loop)*(big?2.0*brightness:1.0*brightness));
		big = !big; // Make the next part of the star the opposite of this part.
	}                    
	glEnd();

	MAKE_FLARE( 0.7,0.2,1.0,1.0,1.0); // Make the flares. Add or Remove some if you like. The MAKE_FLARE macro will only work right here.
	MAKE_FLARE( 0.6,0.3,1.0,0.6,0.6); // It looks better with a chaotic size. I don't know why, though.
	MAKE_FLARE( 0.4,0.4,1.0,1.0,1.0); // Have mine coloured to make a rainbow.
	MAKE_FLARE( 0.3,0.6,0.8,0.8,0.6);
	MAKE_FLARE( 0.2,0.5,1.0,1.0,1.0);
	MAKE_FLARE(-0.1,0.4,0.6,1.0,0.6);
	MAKE_FLARE(-0.2,0.3,1.0,1.0,1.0);
	MAKE_FLARE(-0.4,0.2,0.6,0.8,0.8);
	MAKE_FLARE(-0.6,0.4,1.0,1.0,1.0);
	MAKE_FLARE(-0.7,0.5,0.6,0.6,1.0);
	MAKE_FLARE(-0.8,0.6,1.0,1.0,1.0);
	MAKE_FLARE(-0.9,0.5,0.8,0.6,0.8);
	MAKE_FLARE(-1.2,0.3,1.0,1.0,1.0);
	glEnable(GL_LIGHTING); // Turn the stuff on that I turned off.
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glPopMatrix(); 
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix(); // Get the old matricies back.
	SWAP;
}    

void keypress(byte key, int x, int y)
{
	switch (key) {
	case 27:	// Esc
		exit(0);
	case 97:	// a (left)
		dir -= 0.5;  // Turn and move.
		break;
	case 100:	// d (right)
		dir += 0.5;  // Turn and move.
		break;
	case 119:	// w (up)
		x += sin(dir/57.29578)*0.05;
		y -= cos(dir/57.29578)*0.05;
		break;
	case 115:	// s (down)
		x -= sin(dir/57.29578)*0.05;
		y += cos(dir/57.29578)*0.05;
		break;
	case 44:	// ,
		if(pitch > -45) pitch -= 0.5;
		break;
	case 46:	// .
		if(pitch < 45) pitch += 0.5;
		break;
	default:
		fprintf(stderr, "key: %d\n", key);
	}
}

void timer(int value)
{
}

