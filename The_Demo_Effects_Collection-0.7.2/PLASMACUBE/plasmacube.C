/* Copyright (C) 2002 W.P. van Paassen - peter@paassen.tmfweb.nl

   This program is free software; you can redistribute it and/or modify it under
   the terms of the GNU General Public License as published by the Free
   Software Foundation; either version 2 of the License, or (at your
   option) any later version.

   This program is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
   for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to the Free
   Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  */

#include <cmath>
#include "tdec.h"
#include "WPCG.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

WP_GLState *state;
WP_Camera *cam;
WP_ObjectManager *manager;
Uint16 heading = 0;
WP_Image *logo;
WP_Image *image;
Uint16 pos1 = 0, pos2 = 0, pos3 = 0, pos4 = 0, tpos1, tpos2, tpos3, tpos4;
int aSin[512];
WP_RGBA colors[256];

void quit( int code )
{      
  /*
   * Quit SDL so we can release the fullscreen
   * mode and restore the previous video settings,
   * etc.
   */
  
  SDL_Quit( );

  TDEC_print_fps();
  
  /* Exit program. */
  exit( code );
}

void handle_key_down( SDL_keysym* keysym )
{
    switch( keysym->sym )
      {
      case SDLK_ESCAPE:
	quit(1);
        break;
      default:
        break;
      }
}

void process_events( void )
{
  /* Our SDL event placeholder. */
  SDL_Event event;
  
  /* Grab all the events off the queue. */
  while( SDL_PollEvent( &event ) ) {
    
    switch( event.type ) {
    case SDL_KEYDOWN:
      /* Handle key presses. */
      handle_key_down( &event.key.keysym );
      break;
    case SDL_QUIT:
      /* Handle quit requests */    
      quit(1);
      break;
    }
  }
}

void draw_screen( void )
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  GLuint id;
  int i, j, x;
  Uint8 index;

  heading += 2;
  heading %= 360;

  // draw plasma 

  tpos4 = pos4;
  tpos3 = pos3;

  for (i = 0; i < image->rows; ++i)
    {
      tpos1 = pos1 + 5;
      tpos2 = pos2 + 3;
	  
      tpos3 &= 511;
      tpos4 &= 511;
	  
      for (j = 0; j < image->columns; ++j)
	{
	  tpos1 &= 511;
	  tpos2 &= 511;
	      
	  x = aSin[tpos1] + aSin[tpos2] + aSin[tpos3] + aSin[tpos4]; //actual plasma calculation
	  
	  index = 128 + (x >> 4); //fixed point multiplication but optimized so basically it says (x * (64 * 1024) / (1024 * 1024)), x is already multiplied by 1024
	  
	  image->setFastPixel(i, j, colors[index]);
	      
	  tpos1 += 5; 
	  tpos2 += 3; 
	}
	  
      tpos4 += 3;
      tpos3 += 1;
    }
           
  // move plasma 
  
  pos1 +=9;
  pos3 +=8;

  //render cube

  state->modelview();
  glPushMatrix();
  glRotatef((float)heading, 1.0, 1.0, 0.0);
  state->enableTexture2D();
  image->setTextureGL(&id);

  glBegin(GL_QUADS);

  //front

  glTexCoord2f(0.0, 0.0);
  glColor3f(0.75, 0.75, 0.75);
  glNormal3f(0.57735, -0.57735, 0.57735);
  glVertex3f(.5, -.5, .5);

  glTexCoord2f(0.0, 1.0);
  glColor3f(0.75, 0.75, 0.75);
  glNormal3f(0.57735, 0.57735, 0.57735);
  glVertex3f(.5, .5, .5);

  glTexCoord2f(1.0, 1.0);
  glColor3f(0.75, 0.75, 0.75);
  glNormal3f(-0.57735, 0.57735, 0.57735);
  glVertex3f(-.5, .5, .5);  

  glTexCoord2f(1.0, 0.0);
  glColor3f(0.75, 0.75, 0.75);
  glNormal3f(-0.57735, -0.57735, 0.57735);
  glVertex3f(-.5, -.5, .5);

  //left

  glTexCoord2f(1.0, 0.0);
  glColor3f(0.75, 0.75, 0.75);
  glNormal3f(-0.57735, -0.57735, 0.57735);
  glVertex3f(-.5, -.5, .5);

  glTexCoord2f(1.0, 1.0);
  glColor3f(0.75, 0.75, 0.75);
  glNormal3f(-0.57735, 0.57735, 0.57735);
  glVertex3f(-.5, .5, .5);

  glTexCoord2f(0.0, 1.0);
  glColor3f(0.75, 0.75, 0.75);
  glNormal3f(-0.57735, 0.57735, -0.57735);
  glVertex3f(-.5, .5, -.5);  

  glTexCoord2f(0.0, 0.0);
  glColor3f(0.75, 0.75, 0.75);
  glNormal3f(-0.57735, -0.57735, -0.57735);
  glVertex3f(-.5, -.5, -.5);
  
  //back
  
  glTexCoord2f(1.0, 0.0);
  glColor3f(0.75, 0.75, 0.75);
  glNormal3f(0.57735, -0.57735, -0.57735);
  glVertex3f(.5, -.5, -.5);

  glTexCoord2f(0.0, 0.0);
  glColor3f(0.75, 0.75, 0.75);
  glNormal3f(-0.57735, -0.57735, -0.57735);
  glVertex3f(-.5, -.5, -.5);

  glTexCoord2f(0.0, 1.0);
  glColor3f(0.75, 0.75, 0.75);
  glNormal3f(-0.57735, 0.57735, -0.57735);
  glVertex3f(-.5, .5, -.5);  

  glTexCoord2f(1.0, 1.0);
  glColor3f(0.75, 0.75, 0.75);
  glNormal3f(0.57735, 0.57735, -0.57735);
  glVertex3f(.5, .5, -.5);
  
  //right

  glTexCoord2f(0.0, 0.0);
  glColor3f(0.75, 0.75, 0.75);
  glNormal3f(0.57735, -0.57735, 0.57735);
  glVertex3f(.5, -.5, .5);

  glTexCoord2f(1.0, 0.0);
  glColor3f(0.75, 0.75, 0.75);
  glNormal3f(0.57735, -0.57735, -0.57735);
  glVertex3f(.5, -.5, -.5);

  glTexCoord2f(1.0, 1.0);
  glColor3f(0.75, 0.75, 0.75);
  glNormal3f(0.57735, 0.57735, -0.57735);
  glVertex3f(.5, .5, -.5);  

  glTexCoord2f(0.0, 1.0);
  glColor3f(0.75, 0.75, 0.75);
  glNormal3f(0.57735, 0.57735, 0.57735);
  glVertex3f(.5, .5, .5);

  //top

  glTexCoord2f(0.0, 1.0);
  glColor3f(0.75, 0.75, 0.75);
  glNormal3f(0.57735, 0.57735, 0.57735);
  glVertex3f(.5, .5, .5);

  glTexCoord2f(0.0, 0.0);
  glColor3f(0.75, 0.75, 0.75);
  glNormal3f(0.57735, 0.57735, -0.57735);
  glVertex3f(.5, .5, -.5);

  glTexCoord2f(0.0, 1.0);
  glColor3f(0.75, 0.75, 0.75);
  glNormal3f(-0.57735, 0.57735, -0.57735);
  glVertex3f(-.5, .5, -.5);  

  glTexCoord2f(1.0, 1.0);
  glColor3f(0.75, 0.75, 0.75);
  glNormal3f(-0.57735, 0.57735, 0.57735);
  glVertex3f(-.5, .5, .5);

  //bottom

  glTexCoord2f(0.0, 1.0);
  glColor3f(0.75, 0.75, 0.75);
  glNormal3f(0.57735, -0.57735, 0.57735);
  glVertex3f(.5, -.5, .5);

  glTexCoord2f(0.0, 0.0);
  glColor3f(0.75, 0.75, 0.75);
  glNormal3f(-0.57735, -0.57735, 0.57735);
  glVertex3f(-.5, -.5, .5);

  glTexCoord2f(0.0, 1.0);
  glColor3f(0.75, 0.75, 0.75);
  glNormal3f(-0.57735, -0.57735, -0.57735);
  glVertex3f(-.5, -.5, -.5);  

  glTexCoord2f(1.0, 1.0);
  glColor3f(0.75, 0.75, 0.75);
  glNormal3f(0.57735, -0.57735, -0.57735);
  glVertex3f(.5, -.5, -.5);
  
  glEnd();
  glPopMatrix();

  glDeleteTextures(1, &id);
  state->disableTexture2D();

  //draw logo

  state->disableDepthTest();
  state->disableCulling();
  state->disableLighting();

  state->projection();
  glPushMatrix();
  glLoadIdentity();
  gluOrtho2D(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT);
      
  state->modelview();
  glPushMatrix();
  glLoadIdentity();

  logo->drawToFrameBuffer();

  glPopMatrix();
  state->projection(); 
  glPopMatrix();
  state->modelview();
      
  state->enableDepthTest();
  state->enableCulling();
  state->enableLighting();

  manager->updateAll();
  
  SDL_GL_SwapBuffers( );
}

void init()
{
  state = WP_GLState::getInstance();

  glShadeModel(GL_SMOOTH);
  state->enableDepthTest();
  glCullFace(GL_BACK);
  state->enableCulling();
	
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);//GL_DECAL
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); //GL_FASTEST
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
  glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
  glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
  glPixelStorei(GL_UNPACK_SWAP_BYTES, GL_FALSE);
	
  manager = WP_ObjectManager::getInstance();
  manager->addLight(WP_Point3D(-10.0, -10.0, 0.0), WP_Color((float)0.0, (float)0.0, (float)0.0), 
		    WP_Color((float)0.05, (float)0.95, (float)0.05), WP_Color((float)0.05, (float)0.95, (float)0.05),
		    WP_Color((float)0.0, (float)0.0, (float)0.0));
		    
  manager->addLight(WP_Point3D(5.0, 5.0, 0.0), WP_Color((float)0.0, (float)0.0, (float)0.0), 
		    WP_Color((float)0.95, (float)0.05, (float)0.95), WP_Color((float)0.95, (float)0.05, (float)0.95),
		    WP_Color((float)0.0, (float)0.0, (float)0.0));
  	    
  cam = WP_Camera::getInstance();
  
  WP_Point3D eye(0.0, 0.0, 2.0);
  WP_Point3D look(0.0, 0.0, 0.0);
  WP_Vector3D up(0.0, 1.0, 0.0);
  
  cam->setFrustumAndCamera(60.0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.1f, 100.0f, eye, look, up);
	
  //set background color
  glClearColor(0.0, 0.0, 0.0, 0.0f);

  image = new WP_Image(128,128); //TEXTURE SIZE HAS GOT TO BE A POWER OF 2

  Uint16 i;
  float rad;
 
  /*create sin lookup table */
  for (i = 0; i < 512; i++)
    {
      rad =  ((float)i * 0.703125) * 0.0174532; /* 360 / 512 * degree to rad, 360 degrees spread over 512 values to be able to use AND 512-1 instead of using modulo 360*/
      aSin[i] = (int)(sinf(rad) * 1024.0f); /*using fixed point math with 1024 as base*/
    }

  logo = new WP_Image("../GFX/tdec-small.pcx");
  logo->rasterpos_x = SCREEN_WIDTH - logo->columns;
  logo->rasterpos_y = SCREEN_HEIGHT;

  // create palette
  for (i = 0; i < 64; ++i)
    {
      colors[i].r = i << 2;
      colors[i].g = 255 - ((i << 2) + 1); 
      colors[i+64].r = 255;
      colors[i+64].g = (i << 2) + 1;
      colors[i+128].r = 255 - ((i << 2) + 1);
      colors[i+128].g = 255 - ((i << 2) + 1);
      colors[i+192].g = (i << 2) + 1; 
    } 

  /*disable events */
  for (i = 0; i < SDL_NUMEVENTS; ++i) {
    if (i != SDL_KEYDOWN && i != SDL_QUIT) {
      SDL_EventState(i, SDL_IGNORE);
    }
  }
  
  SDL_ShowCursor(SDL_DISABLE);
}

int main( int argc, char* argv[] )
{
  if (argc > 1) {
    cout << "Plasma Cube - W.P. van Paassen - 2002" << endl;
    return -1;
  }
  
  TDEC_init_video();

  SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
  SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 5 );
  SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );
  SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
  SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
  
  if (!TDEC_set_video_GL(SCREEN_WIDTH, SCREEN_HEIGHT, 16, SDL_DOUBLEBUF | SDL_HWACCEL | SDL_HWSURFACE | SDL_HWPALETTE  
			 /*| SDL_FULLSCREEN*/))
    quit(1);
  
  TDEC_init_timer();
  TDEC_set_fps(60);
  
  SDL_WM_SetCaption("Plasma Cube effect ", "");
  
  WP_Init* wpcg = new WP_Init(argc, argv);
  wpcg->vSetViewPort(0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);	
  init();

  /* time based demo loop */
  while( 1 ) 
    {
      TDEC_new_time();
    
      process_events();

      if (TDEC_fps_ok()) 
	{
	  draw_screen();
	}
    }
  
  return 0; /* never reached */
}
