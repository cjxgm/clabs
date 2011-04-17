/* Copyright (C) 2003 W.P. van Paassen - peter@paassen.tmfweb.nl

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
WP_DynamicObject* demons[8];
WP_DynamicObject* weapons[8];
WP_Object *current;
WP_Object *current_demon;
WP_Object *current_weapon;
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
	current_demon->setAnimationCategory(categories[categoryindex]);
	current_weapon->setAnimationCategory(categories[categoryindex]);
        break;
      case SDLK_LEFT:
	categoryindex--;
	if (categoryindex < 0)
	  categoryindex = ncategories - 1;
	current_demon->setAnimationCategory(categories[categoryindex]);
	current_weapon->setAnimationCategory(categories[categoryindex]);
        break;
      default:
        break;
      }
}

void handle_mouse_down (SDL_MouseButtonEvent *button)
{
  WP_Object *now =  manager->pickObject(button->x, button->y);
  if (now)
    {
      current = now;

      // is the picked object a demon or a weapon?
      int pos = current->object_name.find("Demon");
      if (pos != string::npos)
	{
	  //demon, so find its weapon
	  int count;
	  for (count = 0; count < 8; ++count)
	    if (demons[count] == current)
	      {
		current_demon = current;
		current_weapon = weapons[count];
		break;
	      }
	}
      else
	{
	  //weapon, so find its demon
	  int count;
	  for (count = 0; count < 8; ++count)
	    if (weapons[count] == current)
	      {
		current_demon = demons[count];
		current_weapon = current;
		break;
	      }
	}

      // set animation category index 
      int i;
      for (i = 0; i < ncategories; ++i)
	{
	  if (current->getAnimationCategory() ==  categories[i])
	    {
	      categoryindex = i;
	      break;
	    }
	}
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
    case SDL_MOUSEBUTTONDOWN:
      handle_mouse_down(&event.button);
      break;
    }
  }
}

void draw_screen()
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

  static char p[10];
  sprintf(p, "fps: %i",rfps);
  draw.vDrawString(GLUT_BITMAP_HELVETICA_18, p, 10,  SCREEN_HEIGHT - 20);
  
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
  
  WP_Point3D eye(.0, 0.0, 0.0);
  WP_Point3D look(0.0, 0.0, -10.0);
  WP_Vector3D up(0.0, 1.0, 0.0);
  
  cam->setFrustumAndCamera(60.0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.1f, 100.0f, eye, look, up);
	
  //set background color
  glClearColor(0.0, 0.0, 0.0, 0.0f);

  logo = new WP_Image("../GFX/tdec-small.pcx");
  logo->rasterpos_x = SCREEN_WIDTH - logo->columns;
  logo->rasterpos_y = SCREEN_HEIGHT;

  Uint16 i;

  /*disable events */
  for (i = 0; i < SDL_NUMEVENTS; ++i) 
    {
    if (i != SDL_KEYDOWN && i != SDL_QUIT && i != SDL_MOUSEBUTTONDOWN && i != SDL_MOUSEBUTTONUP) 
      {
	SDL_EventState(i, SDL_IGNORE);
      }
    }

  // create demons and place them in a circle, facing the center
  demons[0] = manager->createDynamicObject(WP_Matrix3D(TRANSLATION_MATRIX,0.0 ,0.0, -10.0) * WP_Matrix3D(Y_ROTATION_MATRIX, 180), "Demon1", "tris1.MD2"); 
  weapons[0] = manager->createDynamicObject(WP_Matrix3D(TRANSLATION_MATRIX,0.0 ,0.0, -10.0) * WP_Matrix3D(Y_ROTATION_MATRIX, 180), "Weapon1", "weapon.MD2"); 
  demons[1] = manager->createDynamicObject(WP_Matrix3D(TRANSLATION_MATRIX,7.07 ,0.0, -7.07) * WP_Matrix3D(Y_ROTATION_MATRIX, 135) , "Demon2", "tris1.MD2"); 
  weapons[1] = manager->createDynamicObject(WP_Matrix3D(TRANSLATION_MATRIX,7.07 ,0.0, -7.07) * WP_Matrix3D(Y_ROTATION_MATRIX, 135), "Weapon2", "weapon.MD2"); 

  demons[2] = manager->createDynamicObject(WP_Matrix3D(TRANSLATION_MATRIX,10.0 ,0.0, 0.0) * WP_Matrix3D(Y_ROTATION_MATRIX, 90), "Demon3", "tris1.MD2"); 
  weapons[2] = manager->createDynamicObject(WP_Matrix3D(TRANSLATION_MATRIX,10.0 ,0.0, 0.0) * WP_Matrix3D(Y_ROTATION_MATRIX, 90), "Weapon3", "weapon.MD2"); 

  demons[3] = manager->createDynamicObject(WP_Matrix3D(TRANSLATION_MATRIX,7.07 ,0.0, 7.07) * WP_Matrix3D(Y_ROTATION_MATRIX, 45), "Demon4", "tris1.MD2"); 
  weapons[3] = manager->createDynamicObject(WP_Matrix3D(TRANSLATION_MATRIX,7.07 ,0.0, 7.07) * WP_Matrix3D(Y_ROTATION_MATRIX, 45), "Weapon4", "weapon.MD2"); 

  demons[4] = manager->createDynamicObject(WP_Matrix3D(TRANSLATION_MATRIX,0.0 ,0.0, 10.0), "Demon5", "tris1.MD2"); 
  weapons[4] = manager->createDynamicObject(WP_Matrix3D(TRANSLATION_MATRIX,0.0 ,0.0, 10.0), "Weapon5", "weapon.MD2"); 

  demons[5] = manager->createDynamicObject(WP_Matrix3D(TRANSLATION_MATRIX,-7.07 ,0.0, 7.07) * WP_Matrix3D(Y_ROTATION_MATRIX, 315), "Demon6", "tris1.MD2"); 
  weapons[5] = manager->createDynamicObject(WP_Matrix3D(TRANSLATION_MATRIX,-7.07 ,0.0, 7.07) * WP_Matrix3D(Y_ROTATION_MATRIX, 315), "Weapon6", "weapon.MD2"); 

  demons[6] = manager->createDynamicObject(WP_Matrix3D(TRANSLATION_MATRIX,-10.0 ,0.0, 0.0) * WP_Matrix3D(Y_ROTATION_MATRIX, 270), "Demon7", "tris1.MD2"); 
  weapons[6] = manager->createDynamicObject(WP_Matrix3D(TRANSLATION_MATRIX,-10.0 ,0.0, 0.0) * WP_Matrix3D(Y_ROTATION_MATRIX, 270), "Weapon7", "weapon.MD2"); 

  demons[7] = manager->createDynamicObject(WP_Matrix3D(TRANSLATION_MATRIX,-7.07 ,0.0, -7.07) * WP_Matrix3D(Y_ROTATION_MATRIX, 225), "Demon8", "tris1.MD2"); 
  weapons[7] = manager->createDynamicObject(WP_Matrix3D(TRANSLATION_MATRIX,-7.07 ,0.0, -7.07) * WP_Matrix3D(Y_ROTATION_MATRIX, 225), "Weapon8", "weapon.MD2"); 
  current = demons[0];
  current_demon = current;
  current_weapon = weapons[0];

  //get animation category for demon and weapon model (these have the same categories, not in the original demon and weapon models however, I changed this according to ID's standard)
  ncategories = current->getAnimationCategories(&categories);

  //set animations
  demons[0]->setAnimationCategory(categories[0]);
  demons[1]->setAnimationCategory(categories[1]);
  demons[2]->setAnimationCategory(categories[2]);
  demons[3]->setAnimationCategory(categories[3]);
  demons[4]->setAnimationCategory(categories[6]);
  demons[5]->setAnimationCategory(categories[7]);
  demons[6]->setAnimationCategory(categories[4]);
  demons[7]->setAnimationCategory(categories[5]);

  weapons[0]->setAnimationCategory(categories[0]);
  weapons[1]->setAnimationCategory(categories[1]);
  weapons[2]->setAnimationCategory(categories[2]);
  weapons[3]->setAnimationCategory(categories[3]);
  weapons[4]->setAnimationCategory(categories[6]);
  weapons[5]->setAnimationCategory(categories[7]);
  weapons[6]->setAnimationCategory(categories[4]);
  weapons[7]->setAnimationCategory(categories[5]);
}

int main( int argc, char* argv[] )
{
  if (argc > 1) {
    cout << "Key frame animated Quake2 Models - W.P. van Paassen - 2003" << endl;
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
  
  SDL_WM_SetCaption("Key frame animated Quake2 Models ", "");
  
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
  	  cam->rotate(0,0.2,0);
  	  manager->updateAll();
	  next = SDL_GetTicks() + TICK_INTERVAL;
	}
    }
  
  return 0; /* never reached */
}
