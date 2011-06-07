/*
 *  "Spinning Quark" OpenGL demo
 *
 *  Copyright (C) 2003,2004,2005  Christophe Devine
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifdef WIN32
#include <windows.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979f
#endif

/* application window title */

char *AppTitle = "Spinning Quark";

/* external OS-dependant functions */

struct htime
{
    unsigned char blob[16];
};

extern float timer( struct htime *t, int reset );

extern void move_mouse( int x, int y );

extern void hide_mouse( void );

/* global data */

#define NB_SPH 3
#define NB_L_X 6
#define NB_L_Y 3

int width, height;

int info;
int frames;
GLuint font;
GLfloat fps;
GLuint texture;
GLfloat dir[NB_SPH][3];
GLfloat phi[NB_SPH][3];
GLfloat rx[NB_SPH];
GLfloat ry[NB_SPH];
GLfloat vx[NB_SPH];
GLfloat vy[NB_SPH];
GLUquadricObj *qobj;
struct htime tv;

/* text drawing routine declaration */

void gl_printf( GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha,
                GLint x, GLint y, GLuint font, const char *format, ... );

/* data initialization function */

int gl_data( void )
{
    int i, j;

    info = 1;

    srand( (unsigned int) time( NULL ) );

    for( i = 0; i < NB_SPH; i++ )
    {
        for( j = 0; j < 3; j++ )
        {
            dir[i][j] = (GLfloat) ( 2 * ( rand() % 2 ) - 1 );
            phi[i][j] = 2.0f * M_PI * (GLfloat) rand() / RAND_MAX;
        }

        rx[i] = 360.0f * (GLfloat) rand() / RAND_MAX;
        ry[i] = 360.0f * (GLfloat) rand() / RAND_MAX;

        vx[i] = 8.0f + 8.0f * (GLfloat) rand() / RAND_MAX;
        vy[i] = vx[i];
    }

    return( 0 );
}

/* gl initialization function */

int gl_init( void )
{
    int i, j, k;
    unsigned char *map;
    double dx, dy, alpha;

    qobj = gluNewQuadric();
    gluQuadricTexture( qobj, GL_TRUE );

    if( ! ( map = (unsigned char *) malloc( 128 * 128 * 3 ) ) )
    {
        perror( "malloc" );
        return( 1 );
    }

    glGenTextures( 1, &texture );

    glBindTexture( GL_TEXTURE_2D, texture );

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

    k = 0;

    for( i = 0; i < 128; i++ )
    {
        dx = (double) ( i - 64 ) / 64.0;

        for( j = 0; j < 128; j++ )
        {
            dy = (double) ( j - 64 ) / 64.0;

            alpha = 0.8f * ( sin( NB_L_X * M_PI * ( dx + 0.00 ) ) + 1.0f )
                         * ( cos( NB_L_Y * M_PI * ( dy + 0.00 ) ) + 1.0f )
                         * ( cos( M_PI * dx ) + 1.0f )
                         * ( cos( M_PI * dx ) + 1.0f ) / 16.0f;

            map[k++] = (unsigned char) ( alpha * 255.0 );

            alpha = 0.8f * ( sin( NB_L_X * M_PI * ( dx + 0.04 ) ) + 1.0f )
                         * ( cos( NB_L_Y * M_PI * ( dy + 0.04 ) ) + 1.0f )
                         * ( cos( M_PI * dx ) + 1.0f )
                         * ( cos( M_PI * dx ) + 1.0f ) / 16.0f;

            map[k++] = (unsigned char) ( alpha * 255.0 );

            alpha = 0.8f * ( sin( NB_L_X * M_PI * ( dx + 0.08 ) ) + 1.0f )
                         * ( cos( NB_L_Y * M_PI * ( dy + 0.08 ) ) + 1.0f )
                         * ( cos( M_PI * dx ) + 1.0f )
                         * ( cos( M_PI * dx ) + 1.0f ) / 16.0f;

            map[k++] = (unsigned char) ( alpha * 255.0 );
        }
    }

    glTexImage2D( GL_TEXTURE_2D, 0, 3, 128, 128, 0,
                  GL_RGB, GL_UNSIGNED_BYTE, map );

    free( map );

    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE );

    hide_mouse();

    return( 0 );
}

/* window drawing function */

void gl_draw( void )
{
    int i, j;
    GLfloat r, g, b;

    glClear( GL_COLOR_BUFFER_BIT );

    if( fps )
    {
        for( i = 0; i < NB_SPH; i++ )
        {
            for( j = 0; j < 3; j++ )
            {
                if( ! ( rand() % ( (int) ( fps * 64.0f ) ) ) )
                {
                    dir[i][j] = (GLfloat) ( rand() % 3 - 1 );
                }
                phi[i][j] += 0.6f * dir[i][j] / fps;
            }

            rx[i] += vx[i] / fps;
            ry[i] += vy[i] / fps;
        }
    }

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    glTranslatef( 0.0f, 0.0f, -6.0f );

    glEnable( GL_TEXTURE_2D );

    for( i = 0; i < NB_SPH; i++ )
    {
        r = (GLfloat) ( ( sin( phi[i][0] ) + 1.1f ) *
                        ( sin( phi[i][0] ) + 1.1f ) ) / 2.0f;

        g = (GLfloat) ( ( sin( phi[i][1] ) + 1.1f ) *
                        ( sin( phi[i][1] ) + 1.1f ) ) / 2.0f;

        b = (GLfloat) ( ( sin( phi[i][2] ) + 1.1f ) *
                        ( sin( phi[i][2] ) + 1.1f ) ) / 2.0f;

        glColor3f( r, g, b );

        glPushMatrix();
        glRotatef( rx[i], 1.0f, 0.0f, 0.0f );
        glRotatef( ry[i], 0.0f, 1.0f, 0.0f );
        gluSphere( qobj, 2.0f, 48, 48 );
        glPopMatrix();
    }

    glDisable( GL_TEXTURE_2D );

    if( fps && info )
    {
        gl_printf( 0.7f, 0.7f, 0.7f, 1.0f, width - 114, height - 40,
                   font, "%5.1f fps", fps );
    }

    glFinish();

    frames++;

    if( timer( &tv, 0 ) >= 0.2f )
    {
        fps = (GLfloat) frames / timer( &tv, 1 );
        frames = 0;
    }
}

/* window resizing function */

void gl_resize( void )
{
    glViewport( 0, 0, width, height );

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective( 50.0, (GLdouble) width / height, 1.0, 100.0 );

    fps = 0.0f;
    frames = 0;
    timer( &tv, 1 );
}

/* event handling function */

void gl_event( int event, int data, int xpos, int ypos )
{
    if( event == 0 )    /* mouse button down */
    {
    }

    if( event == 1 )    /* mouse button up */
    {
    }

    if( event == 2 )    /* mouse move */
    {
    }

    if( event == 3 )    /* key down */
    {
    }

    if( event == 4 )    /* key up */
    {
        if( data == ' ' )
        {
            info ^= 1;
        }
    }
}

/* text drawing routine */

void gl_printf( GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha,
                GLint x, GLint y, GLuint font, const char *format, ... )
{
    va_list argp;
    char text[256];

    va_start( argp, format );
    vsprintf( text, format, argp );
    va_end( argp );

    glMatrixMode( GL_PROJECTION );
    glPushMatrix();

        glLoadIdentity();
        gluOrtho2D( 0.0, (GLdouble) width,
                    0.0, (GLdouble) height );

        glMatrixMode( GL_MODELVIEW );
        glLoadIdentity();

        glColor4f( red, green, blue, alpha );
        glRasterPos2i( x, y );
        glListBase( font );
        glCallLists( strlen( text ), GL_UNSIGNED_BYTE, text );

    glMatrixMode( GL_PROJECTION );
    glPopMatrix();
}
