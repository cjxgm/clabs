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
WP_Image *tex;
scalar aSin[360];
scalar aCos[360];
GLuint id;
scalar delta = 0.0;
Uint16 dindex = 0;
Uint8 light_speed = 2;
scalar speed = 0.02;
bool fog_enable = true;
bool spotlight_enable = true;

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
      case SDLK_F1:
	if (fog_enable)
	  {
	    glDisable(GL_FOG);
	  }
	else
	    glEnable(GL_FOG);
	fog_enable = !fog_enable;
	break;
      case SDLK_F2:
	if (spotlight_enable)
	  {
	    state->disableLighting();
	  }
	else
	  state->enableLighting();
	spotlight_enable = !spotlight_enable;
	break;
      case SDLK_F3:
	light_speed++;
	light_speed%=20;
	break;
      case SDLK_F4:
	speed += 0.01;
	if (speed > 0.1)
	  speed = 0.01;
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
  
  Uint8 j;
  Uint16 i;
  scalar d1 = 1.0 + delta;

  //draw tunnel
  state->enableTexture2D();

  for (j = 0; j < 14; ++j)
    {
      for (i = 0; i < 350; i += 10)
	{
	  glBegin(GL_QUADS);
	  glColor3f(1.0, 1.0, 1.0);
	  glTexCoord2f(delta, delta);
	  glNormal3f(-aSin[i], -aCos[i], 0.0);
	  glVertex3f(aSin[i], aCos[i], -j);
	  glColor3f(1.0, 1.0, 1.0);
	  glTexCoord2f(delta, d1);
	  glNormal3f(-aSin[i], -aCos[i], 0.0);
	  glVertex3f(aSin[i], aCos[i], -j - 1.0);
	  glColor3f(1.0, 1.0, 1.0);
	  glTexCoord2f(d1, d1);
	  glNormal3f(-aSin[i + 10], -aCos[i + 10], 0.0);
	  glVertex3f(aSin[i + 10], aCos[i + 10], -j - 1.0);
	  glColor3f(1.0, 1.0, 1.0);
	  glTexCoord2f(d1, delta);
	  glNormal3f(-aSin[i + 10], -aCos[i + 10], 0.0);
	  glVertex3f(aSin[i + 10], aCos[i + 10], -j);
	  glEnd();
	}
      glBegin(GL_QUADS);
      glColor3f(1.0, 1.0, 1.0);
      glTexCoord2f(delta, delta);
      glNormal3f(-aSin[350], -aCos[350], 0.0);
      glVertex3f(aSin[350], aCos[350], -j);
      glColor3f(1.0, 1.0, 1.0);
      glTexCoord2f(delta, d1);
      glNormal3f(-aSin[350], -aCos[350], 0.0);
      glVertex3f(aSin[350], aCos[350], -j - 1.0);
      glColor3f(1.0, 1.0, 1.0);
      glTexCoord2f(d1, d1);
      glNormal3f(-aSin[0], -aCos[0], 0.0);
      glVertex3f(aSin[0], aCos[0], -j - 1.0);
      glColor3f(1.0, 1.0, 1.0);
      glTexCoord2f(d1, delta);
      glNormal3f(-aSin[0], -aCos[0], 0.0);
      glVertex3f(aSin[0], aCos[0], -j);
      glEnd();
    }
  
  state->disableTexture2D();

  delta += speed;
  if (delta >= 1.0)
    delta -= 1.0;

  GLfloat dir[] = {aSin[dindex], aCos[dindex], -2.0};
  glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, dir);

  dindex+= light_speed;
  dindex %=360;

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
  manager->addLight(WP_Point3D(0.0, 0.0, 0.0), WP_Color((float)0.0, (float)0.0, (float)0.0), 
		    WP_Color((float)1.0, (float)1.0, (float)0.0), WP_Color((float)1.0, (float)1.0, (float)0.0),
		    WP_Color((float)0.0, (float)0.0, (float)0.0), false);
		     

  cam = WP_Camera::getInstance();

  WP_Point3D eye(0.0, 0.0, 2.0);
  WP_Point3D look(0.0, 0.0, 0.0);
  WP_Vector3D up(0.0, 1.0, 0.0);
  
  cam->setFrustumAndCamera(60.0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.1f, 100.0f, eye, look, up);
	
  Uint16 i,j;
  float rad;

  tex = new WP_Image("../TEXTURES/tex35.pcx");
  tex->setTextureGL(&id, GL_REPEAT, GL_REPEAT, GL_NEAREST, GL_NEAREST, true);
 
  glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 90.0);
  glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 4.0);

  GLfloat fcolor[] = {0.0, 0.0, 0.0};
  glEnable(GL_FOG);
  glFogf(GL_FOG_MODE, GL_EXP);
  glFogf(GL_FOG_DENSITY, 0.285);
  glFogfv(GL_FOG_COLOR, fcolor);

  //create sin lookup table 
  for (i = 0; i < 360; i++)
    {
      rad =  (float)i * 0.0174532; 
      aSin[i] = sin(rad) * 2.0f;
      aCos[i] = cos(rad) * 2.0f;
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
  if (argc > 1) 
    {
      cout << "Retro Static Tunnel - W.P. van Paassen - 2002" << endl;
      return -1;
    }

  cout << "Toggle F1 to disable/enable Fog" << endl;
  cout << "Toggle F2 to disable/enable Spot Light" << endl;
  cout << "Toggle F3 to increase Spot Light Speed" << endl;
  cout << "Toggle F4 to increase scroll Speed" << endl;

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
  
  SDL_WM_SetCaption("Tunnel effect", "");
  
  WP_Init* wpcg = new WP_Init(argc, argv);
  wpcg->vSetViewPort(0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);	
  init();

  TDEC_set_fps(100);

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
