/*
 *  "Shining Cubes" OpenGL demo
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

char *AppTitle = "Shining Cubes";

/* external OS-dependant functions */

struct htime
{
    unsigned char blob[16];
};

extern float timer( struct htime *t, int reset );

extern void move_mouse( int x, int y );

extern void hide_mouse( void );

/* global data */

int width, height;

int info;
int bmode;
int rdown;
int _done;
int frames;
GLuint font;
GLfloat fps;
GLfloat alpha;
GLfloat rx, ry;
GLfloat vx, vy;
GLfloat dir[8][3];
GLfloat phi[8][3];
struct htime tv;

/* text drawing routine declaration */

void gl_printf( GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha,
                GLint x, GLint y, GLuint font, const char *format, ... );

/* data initialization function */

int gl_data( void )
{
    int i, j;

    info  = 1;
    bmode = 1;
    rdown = 0;
    _done = 0;

    alpha = 0.11f;

    rx = 0.0f; ry =  0.0f;
    vx = 8.0f; vy = 32.0f;

    srand( (unsigned int) time( NULL ) );

    for( i = 0; i < 8; i++ )
    {
        for( j = 0; j < 3; j++ )
        {
            dir[i][j] = (GLfloat) ( rand() % 3 ) - 1;
            phi[i][j] = 2.0f * M_PI * (GLfloat) rand() / RAND_MAX;
        }
    }

    return( 0 );
}

/* gl initialization function */

int gl_init( void )
{
    glEnable( GL_BLEND );

    glBlendFunc( GL_SRC_ALPHA, ( bmode ) ?
                 GL_ONE : GL_ONE_MINUS_SRC_ALPHA );

    move_mouse( width, height );

    return( 0 );
}

/* window drawing function */

void gl_draw( void )
{
    int i, j;
    GLfloat len, lmax, r[8], g[8], b[8], a;

    glClear( GL_COLOR_BUFFER_BIT );

    for( i = 0; i < 8; i++ )
    {
        for( j = 0; j < 3; j++ )
        {
            if( fps )
            {
                if( ! ( rand() % (int) ( fps * 16.0f ) ) )
                {
                    dir[i][j] = (GLfloat) ( rand() % 3 ) - 1;
                }

                phi[i][j] += dir[i][j] / fps;
            }
        }
    }

    for( i = 0; i < 8; i++ )
    {
        r[i] = ( (GLfloat) sin( phi[i][0] ) + 1.0f ) / 2.0f;
        g[i] = ( (GLfloat) sin( phi[i][1] ) + 1.0f ) / 2.0f;
        b[i] = ( (GLfloat) sin( phi[i][2] ) + 1.0f ) / 2.0f;
    }

    if( fps )
    {
        rx += vx / fps;
        ry += vy / fps;
    }

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    glTranslatef( 0.0f, 0.0f, -8.0f );

    glRotatef( rx, 1.0f, 0.0f, 0.0f );
    glRotatef( ry, 0.0f, 1.0f, 0.0f );

    a = alpha;

    lmax = 1.4f + 0.12f * bmode;

    glBegin( GL_QUADS );

    for( len = lmax; len > 0.0f; len -= 0.12f )
    {
        glColor4f( r[0], g[0], b[0], a ); glVertex3f( -len, -len, -len );
        glColor4f( r[1], g[1], b[1], a ); glVertex3f( -len, -len,  len );
        glColor4f( r[2], g[2], b[2], a ); glVertex3f( -len,  len,  len );
        glColor4f( r[3], g[3], b[3], a ); glVertex3f( -len,  len, -len );

        glColor4f( r[4], g[4], b[4], a ); glVertex3f(  len, -len, -len );
        glColor4f( r[5], g[5], b[5], a ); glVertex3f(  len, -len,  len );
        glColor4f( r[6], g[6], b[6], a ); glVertex3f(  len,  len,  len );
        glColor4f( r[7], g[7], b[7], a ); glVertex3f(  len,  len, -len );

        glColor4f( r[0], g[0], b[0], a ); glVertex3f( -len, -len, -len );
        glColor4f( r[1], g[1], b[1], a ); glVertex3f( -len, -len,  len );
        glColor4f( r[5], g[5], b[5], a ); glVertex3f(  len, -len,  len );
        glColor4f( r[4], g[4], b[4], a ); glVertex3f(  len, -len, -len );

        glColor4f( r[3], g[3], b[3], a ); glVertex3f( -len,  len, -len );
        glColor4f( r[2], g[2], b[2], a ); glVertex3f( -len,  len,  len );
        glColor4f( r[6], g[6], b[6], a ); glVertex3f(  len,  len,  len );
        glColor4f( r[7], g[7], b[7], a ); glVertex3f(  len,  len, -len );

        glColor4f( r[0], g[0], b[0], a ); glVertex3f( -len, -len, -len );
        glColor4f( r[3], g[3], b[3], a ); glVertex3f( -len,  len, -len );
        glColor4f( r[7], g[7], b[7], a ); glVertex3f(  len,  len, -len );
        glColor4f( r[4], g[4], b[4], a ); glVertex3f(  len, -len, -len );

        glColor4f( r[1], g[1], b[1], a ); glVertex3f( -len, -len,  len );
        glColor4f( r[2], g[2], b[2], a ); glVertex3f( -len,  len,  len );
        glColor4f( r[6], g[6], b[6], a ); glVertex3f(  len,  len,  len );
        glColor4f( r[5], g[5], b[5], a ); glVertex3f(  len, -len,  len );
    }

    glEnd();

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
    if( rdown )
    {
        vx = 256.0f * (GLfloat) ( ypos - height / 2 ) / height;
        vy = 256.0f * (GLfloat) ( xpos - width  / 2 ) / width;
    }

    if( event == 0 )    /* mouse button down */
    {
        if( data == 1 ) /* right button */
        {
            rdown = 1;
        }
    }

    if( event == 1 )    /* mouse button up */
    {
        if( data == 0 ) /* left button */
        {
            bmode ^= 1;
            alpha = ( bmode ) ? 0.11f : 0.40f;
            glBlendFunc( GL_SRC_ALPHA, ( bmode ) ?
                         GL_ONE : GL_ONE_MINUS_SRC_ALPHA );
        }

        if( data == 1 ) /* right button */
        {
            rdown = 0;
        }
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
