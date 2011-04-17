/* Copyright (C) 2002-2003 W.P. van Paassen - peter@paassen.tmfweb.nl

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
#define TICK_INTERVAL 18

WP_Image *logo;
WP_GLState *state;
WP_Camera *cam;
WP_Draw_2D draw;
WP_ObjectManager *manager;
WP_DynamicObject *demon;
WP_DynamicObject *weapon;
scalar heading = 0.0;
string *categories;
unsigned short ncategories;
short categoryindex = 0;
Uint32 fps = 0;
Uint32 rfps = 0;

void quit( int code )
{      
  /*
   * Quit SDL so we can release the fullscreen
   * mode and restore the previous video settings,
   * etc.
   */
  
  SDL_Quit( );

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
      case SDLK_RIGHT:
	categoryindex++;
	categoryindex %= ncategories;
	demon->setAnimationCategory(categories[categoryindex]);
	weapon->setAnimationCategory(categories[categoryindex]);
        break;
      case SDLK_LEFT:
	categoryindex--;
	if (categoryindex < 0)
	  categoryindex = ncategories - 1;
	demon->setAnimationCategory(categories[categoryindex]);
	weapon->setAnimationCategory(categories[categoryindex]);
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

  //draw text

  state->disableDepthTest();
  state->disableLighting();
  state->disableCulling();
        
  state->projection();
  glPushMatrix();
  glLoadIdentity();
  gluOrtho2D(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT);
      
  state->modelview();
  glPushMatrix();
  glLoadIdentity();
  glColor3f(0.0f, 1.0f, 0.0f);

  draw.vDrawString(GLUT_BITMAP_HELVETICA_18, "Animation sequence: " + categories[categoryindex], 10,  SCREEN_HEIGHT - 20);
  draw.vDrawString(GLUT_BITMAP_HELVETICA_18, "Use left or right arrow to change sequence", 10,  SCREEN_HEIGHT - 40);
  static char p[10];
  sprintf(p, "fps: %i",rfps);
  draw.vDrawString(GLUT_BITMAP_HELVETICA_18, p, 10,  SCREEN_HEIGHT - 60);
  
  logo->drawToFrameBuffer();

  glPopMatrix();
  state->projection(); 
  glPopMatrix();
  state->modelview();
      
  state->enableDepthTest();
  state->enableLighting();	
  state->enableCulling();

  manager->drawObjects();

  SDL_GL_SwapBuffers( );
}

void init()
{
  state = WP_GLState::getInstance();

  WP_TextureManager::getInstance()->mipmapping = true;

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

  manager->addLight(WP_Point3D(1.0, 1.0, 0.0), WP_Color((float)0.0, (float)0.0, (float)0.0), 
		    WP_Color((float)0.75, (float)0.75, (float)0.75), WP_Color((float)0.0, (float)0.0, (float)0.0),
		    WP_Color((float)0.0, (float)0.0, (float)0.0));

  cam = WP_Camera::getInstance();
  
  WP_Point3D eye(0.0, 0.0, 7.0);
  WP_Point3D look(0.0, 0.0, 0.0);
  WP_Vector3D up(0.0, 1.0, 0.0);
  
  cam->setFrustumAndCamera(60.0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.1f, 100.0f, eye, look, up);
	
  //set background color
  glClearColor(0.0, 0.0, 0.0, 0.0f);

  logo = new WP_Image("../GFX/tdec-small.pcx");
  logo->rasterpos_x = SCREEN_WIDTH - logo->columns;
  logo->rasterpos_y = SCREEN_HEIGHT;

  Uint16 i;
  /*disable events */
  for (i = 0; i < SDL_NUMEVENTS; ++i) {
    if (i != SDL_KEYDOWN && i != SDL_QUIT) {
      SDL_EventState(i, SDL_IGNORE);
    }
  }
  
  SDL_ShowCursor(SDL_DISABLE);

  // add quake2 demon and weapon model  
  demon = manager->createDynamicObject(WP_Matrix3D(), "Demon", "tris1.MD2"); 
  weapon = manager->createDynamicObject(WP_Matrix3D(), "Demon_Weapon", "weapon.MD2"); 

  //get animation category for demon and weapon model (these have the same categories, not in the original demon and weapon models however, I changed this according to ID's standard)

  ncategories = demon->getAnimationCategories(&categories);
}

int main( int argc, char* argv[] )
{
  if (argc > 1) {
    cout << "Retro Quake2 Model - W.P. van Paassen - 2002" << endl;
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
  
  SDL_WM_SetCaption("Retro Quake2 Model effect ", "");
  
  WP_Init* wpcg = new WP_Init(argc, argv);
  wpcg->vSetViewPort(0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);	
  init();

  Uint32 ntime, time = SDL_GetTicks();
  Uint32 next = SDL_GetTicks() + TICK_INTERVAL; 

  /* time based demo loop */
  while( 1 ) 
    {
     if ((ntime = SDL_GetTicks()) < next)
     {
	     draw_screen();
	     if (ntime - time >= 1000)
	     {
		     rfps = fps;//(Uint32)(((float)fps / (ntime - time)) * 1000);
		     fps = 0;
		     time = SDL_GetTicks();
	     }
	     else
	     	fps++;
     }
     else
	{
     	  process_events();
  	  demon->setNewHeading(heading);
  	  weapon->setNewHeading(heading);
  	  heading += 0.6f;
  	  if (heading >= 360.0f)
    		heading -= 360.0f;
  	  manager->updateAll();
	  next = SDL_GetTicks() + TICK_INTERVAL;
	}
    }
  
  return 0; /* never reached */
}
