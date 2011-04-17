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
#if QUICKTIME
#include "quicktime/quicktime.h"
#endif
#include "tdec.h"
#include "WPCG.h"

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768 
#define TICK_INTERVAL 18
#define MAX_DEPTH 6

WP_Image *logo;
WP_GLState *state;
WP_Camera *cam;
WP_Draw_2D draw;
WP_ObjectManager *manager;
WP_Object* demons[8];
WP_Object* weapons[8];
WP_Object *current;
string *categories;
unsigned short ncategories;
short categoryindex = 0;
Uint32 fps = 0;
Uint32 rfps = 0;
bool trace = false;
bool quit_it = false;
  scalar ambient_red = 0.35;
  scalar ambient_green = 0.35;
  scalar ambient_blue = 0.35;
  WP_Material gold;
  WP_Point3D light(6.0,8.0,2.0);
#if QUICKTIME
quicktime_t * mfile;
#endif


void quit( int code )
{      
  /*
   * Quit SDL so we can release the fullscreen
   * mode and restore the previous video settings,
   * etc.
   */
  
#if QUICKTIME
  quicktime_close(mfile);
#endif
  SDL_Quit( );

  /* Exit program. */
  exit( code );
}

void handle_key_down( SDL_keysym* keysym )
{
    switch( keysym->sym )
      {
      case SDLK_ESCAPE:
      quit_it = true;
        break;
      default:
	trace = true;
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
	quit_it = true;
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
  //state->enableLighting();	
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

  manager->addLight(WP_Point3D(0.0, 0.0, 0.0), WP_Color((float)0.0, (float)0.0, (float)0.0), 
		    WP_Color((float)0.75, (float)0.75, (float)0.75), WP_Color((float)0.0, (float)0.0, (float)0.0),
		    WP_Color((float)0.0, (float)0.0, (float)0.0));

  cam = WP_Camera::getInstance();
  
  WP_Point3D eye(0.0, 0.0, 3.0);
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

  demons[0] = manager->createDynamicObject(WP_Matrix3D(TRANSLATION_MATRIX,0.0 ,-1.7, -8.0) * WP_Matrix3D(Y_ROTATION_MATRIX, 180), "Demon1", "tris1.MD2"); 
  weapons[0] = manager->createDynamicObject(WP_Matrix3D(TRANSLATION_MATRIX,0.0 ,-1.7, -8.0) * WP_Matrix3D(Y_ROTATION_MATRIX, 180), "Weapon2", "weapon.MD2"); 
  demons[1] = manager->createDynamicObject(WP_Matrix3D(TRANSLATION_MATRIX, -1.0, 1.0, -8.0), "UFO", "ufo.MD2"); 
  demons[2] = manager->createDynamicObject(WP_Matrix3D(TRANSLATION_MATRIX, 3.0, 2.0, -10.0), "UFO", "ufo.MD2"); 
  demons[3] = manager->createStaticObject(WP_Matrix3D(TRANSLATION_MATRIX, 0.0, 6.0, -20.0), "WP_SQUARE", "wall"); 
  demons[4] = manager->createStaticObject(WP_Matrix3D(TRANSLATION_MATRIX, 0.0, -4.0, -10.0) * WP_Matrix3D(X_ROTATION_MATRIX, -90), "WP_SQUARE", "wall"); 
  demons[5] = manager->createStaticObject(WP_Matrix3D(TRANSLATION_MATRIX, -10.0, 6.0, -20.0) * WP_Matrix3D(Y_ROTATION_MATRIX, 90) , "WP_SQUARE", "wall"); 
  demons[6] = manager->createStaticObject(WP_Matrix3D(TRANSLATION_MATRIX, 10.0, 6.0, -20.0) * WP_Matrix3D(Y_ROTATION_MATRIX, -90) , "WP_SQUARE", "wall"); 
  current = weapons[0];

  //set specular 

  demons[0]->model->material.specular = WP_Color(0.0f,0.1f,0.1f);
  weapons[0]->model->material.specular = WP_Color(0.0f,0.4f,0.4f);
  demons[1]->model->material.specular = WP_Color(0.6f,0.9f,0.9f);
  demons[2]->model->material.specular = WP_Color(0.6f,0.6f,0.6f);
  demons[3]->model->material.specular = WP_Color(0.6f,0.6f,0.6f);
  demons[4]->model->material.specular = WP_Color(0.6f,0.6f,0.6f);
  demons[5]->model->material.specular = WP_Color(0.6f,0.6f,0.6f);
  demons[6]->model->material.specular = WP_Color(0.6f,0.6f,0.6f);

  //get animation category for demon and weapon model (these have the same categories, not in the original demon and weapon models however, I changed this according to ID's standard)
  ncategories = current->getAnimationCategories(&categories);

  //set animations
  //demons[0]->setAnimationCategory(categories[0]);

  weapons[0]->setAnimationCategory(categories[0]);

}

WP_Color rayTrace(const WP_Ray3D& r, unsigned char recursion_depth)
{

			WP_Color final;
			WP_RayHitPoint h = manager->castRay(r, recursion_depth == 0);
			if (h.obj)
			{
				if (h.color.components[0] == 2.0)
				{
					h.color.components[0] = ambient_red;
					h.color.components[1] = ambient_green;
					h.color.components[2] = ambient_blue;
				}
			   //compute ambient light
			   	final.components[0] = ambient_red * h.color.components[0] + ambient_red * gold.ambient.components[0];
			   	final.components[1] = ambient_green * h.color.components[1] + ambient_green * gold.ambient.components[1];
			   	final.components[2] = ambient_blue * h.color.components[2] + ambient_blue * gold.ambient.components[2];

			   //check if hitpoint is in shadow
			   if (!manager->inShadow(h.hitpoint, light))
			   {
				/* do Lambert */
				WP_Vector3D light_normal(light-h.hitpoint);
				light_normal.normalize();
				scalar angle = h.normal * light_normal; 
				if (angle > 0.0f)
				{
					final.components[0] += h.color.components[0] * gold.diffuse.components[0] * angle;
					final.components[1] += h.color.components[1] * gold.diffuse.components[1] * angle;
					final.components[2] += h.color.components[2] * gold.diffuse.components[2] * angle;
				}
				//do phong
				WP_Vector3D halfway = (light - h.hitpoint) - r.direction;
				halfway.normalize();
				angle = halfway * h.normal;	 
				if (angle > 0.0f)
				{
					scalar phong = pow(angle,gold.specularExponent); 
					final.components[0] += gold.specular.components[0] * gold.specular.components[0] * phong;
					final.components[1] += gold.specular.components[1] * gold.specular.components[1] * phong;
					final.components[2] += gold.specular.components[2] * gold.specular.components[2] * phong;
				}
			   }
				if (recursion_depth == MAX_DEPTH)
					return final;
			   //do reflections

			  if (h.obj->model->material.specular.components[0] > 0.0)
			  {
			   	WP_Ray3D reflection = r.createReflectedRay(h.normal);	
			   	reflection.start = h.hitpoint;
			   	//reflection.direction.normalize();
			   	WP_Color rc = rayTrace(reflection, recursion_depth + 1);
			   	final.components[0] += rc.components[0] * h.obj->model->material.specular.components[0];
			   	final.components[1] += rc.components[1] * h.obj->model->material.specular.components[0];
			   	final.components[2] += rc.components[2] * h.obj->model->material.specular.components[0];
			  }
			}
	return final;
} 

int main( int argc, char* argv[] )
{
  if (argc > 1) {
    cout << "RayTracer" << endl;
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
  
  SDL_WM_SetCaption("RayTracer, press a key to start tracing ", "");
  
  WP_Init* wpcg = new WP_Init(argc,argv);
  wpcg->vSetViewPort(0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);	
  init();
  Uint32 ntime, time = SDL_GetTicks();
  Uint32 next = SDL_GetTicks() + TICK_INTERVAL; 
#if QUICKTIME
  mfile = quicktime_open("test.mov",0,1);
  if (!mfile)
	return 1;
  quicktime_set_video(mfile,1,SCREEN_WIDTH,SCREEN_HEIGHT,24.0,QUICKTIME_JPEG);
  int q = 60;
  quicktime_set_parameter(mfile, "jpeg_quality",&q);
  if (!quicktime_supported_video(mfile,0))
	return 1;
  if (!quicktime_writes_cmodel(mfile,9,0))
	return 1;
  quicktime_set_cmodel(mfile, 9);
  
//  quicktime_set_depth(mfile,24,0);
  unsigned char* buffer[SCREEN_HEIGHT];
  for(int e=0; e < SCREEN_HEIGHT; ++e)
	buffer[e] = new unsigned char[SCREEN_WIDTH * 3];
#endif
  gold.ambient.components[0] = 0.24725;
  gold.ambient.components[1] = 0.1995;
  gold.ambient.components[2] = 0.0745;
  gold.diffuse.components[0] = 0.75164;
  gold.diffuse.components[1] = 0.60648;
  gold.diffuse.components[2] = 0.22648;
  gold.specular.components[0] = 0.628281;
  gold.specular.components[1] = 0.555802;
  gold.specular.components[2] = 0.366065;
  gold.specularExponent = 51.2;

  /* time based loop */
 while(true)
{
#if 0
	  next = SDL_GetTicks() + TICK_INTERVAL;
  while( true ) 
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
	  //if (trace && manager->notInterpolatedFrame())
	//	break;
#endif
     	  process_events();
	  if (quit_it)
		quit(0);
  	  cam->slide(0.00,0.02,0.0);
  	  cam->rotate(-0.10,0.00,0.00);
	  light = cam->eye;
  	  //manager->updateAll();
	  //next = SDL_GetTicks() + TICK_INTERVAL;
	  //break;
//	}
 //   }

  /* ray trace scene */
  cout << "RAYTRACING FRAME.... CASTING RAYS ..." << endl;

  state->disableDepthTest();
  state->disableLighting();
  state->disableCulling();
        
  state->modelview();
  glLoadIdentity();
  state->projection();
  glLoadIdentity();
  gluOrtho2D(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT);
      
  //WP_Light* l = manager->getLight(0);
  //WP_Point3D light = *(l->getPointPosition()); 

	
  for (int x = 0; x < SCREEN_WIDTH; ++x)
	{
		for (int y = 0; y < SCREEN_HEIGHT; ++y)
		{
			WP_Ray3D ray = cam->createRayForTracing(x, y);
			WP_Color c = rayTrace(ray,0); 
			glColor3fv(c.components);
			glRecti(x,y,x+1,y+1);
#if QUICKTIME
			*(buffer[(SCREEN_HEIGHT - 1 - y)] + x * 3) = c.fromFloatToByte(0);
			*(buffer[(SCREEN_HEIGHT - 1 - y)] + (x * 3 + 1)) = c.fromFloatToByte(1); 
			*(buffer[(SCREEN_HEIGHT - 1 - y)] + (x * 3 + 2)) = c.fromFloatToByte(2); 
#endif
		}
	}

  SDL_GL_SwapBuffers( );
#if QUICKTIME
	quicktime_encode_video(mfile,buffer ,0);
#endif

//  while(!quit_it)
//	{
//		process_events();
//		SDL_Delay(1000);
//	}
//  quit(0);
}  
  return 0; /* never reached */
}

