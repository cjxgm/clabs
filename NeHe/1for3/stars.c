/*
 *  "Spiraling Stars" OpenGL demo
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

char *AppTitle = "Spiraling Stars";

/* external OS-dependant functions */

struct htime
{
    unsigned char blob[16];
};

extern float timer( struct htime *t, int reset );

extern void move_mouse( int x, int y );

extern void hide_mouse( void );

/* global data */

int info;
int width;
int height;
int a_done;
int frames;
GLuint font;
GLfloat fps;
GLuint texture;
GLfloat **atoms;
struct htime tv;

/* particle engine constants */

#define NB_ATOMS 1024

#define CREATE_PARTICLE(p)                                      \
{                                                               \
    p[1] = 0.15f - 0.3f * (GLfloat) rand() / RAND_MAX;          \
    p[2] = 0.15f - 0.3f * (GLfloat) rand() / RAND_MAX;          \
    p[3] = 0.50f + 1.0f * (GLfloat) rand() / RAND_MAX;          \
    p[4] = 90.0f * (GLfloat) rand() / RAND_MAX;                 \
    p[0] = p[3] * 40.0f;                                        \
    do {                                                        \
        p[5]  = 1.1f * (GLfloat) rand() / RAND_MAX;             \
        p[6]  = 1.1f * (GLfloat) rand() / RAND_MAX;             \
        p[7]  = 1.1f * (GLfloat) rand() / RAND_MAX;             \
    } while( p[5] < 0.90f && p[6] < 0.90f && p[7] < 0.90f );    \
}

/* text drawing routine declaration */

void gl_printf( GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha,
                GLint x, GLint y, GLuint font, const char *format, ... );

/* data initialization function */

int gl_data( void )
{
    int i;

    info = 1;
    a_done = 0;

    srand( (unsigned int) time( NULL ) );

    atoms = (GLfloat **) malloc( NB_ATOMS * sizeof( GLfloat * ) );

    if( atoms == NULL )
    {
        perror( "malloc" );
        return( 1 );
    }

    for( i = 0; i < NB_ATOMS; i++)
    {
        atoms[i] = (GLfloat *) malloc( 8 * sizeof( GLfloat ) );

        if( atoms[i] == NULL )
        {
            perror( "malloc" );
            return( 1 );
        }

        CREATE_PARTICLE( atoms[i] );
    }

    return( 0 );
}

/* gl initialization function */

int gl_init( void )
{
    int i, j, k;
    double dx, dy, alpha;
    unsigned char *map, c;

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
            alpha = 0.9 * cos( M_PI - M_PI / ( 1.3 + fabs( dx + dy ) ) )
                  + 0.9 * cos( M_PI - M_PI / ( 1.3 + fabs( dx - dy ) ) );
            if( alpha < 0.0 ) alpha = 0.0;
            if( alpha > 1.0 ) alpha = 1.0;
            c = (unsigned char) ( alpha * 255.0 );
            map[k++] = c;
            map[k++] = c;
            map[k++] = c;
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
    int i;
    GLfloat t, x, y, z;

    glClear( GL_COLOR_BUFFER_BIT );

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    glEnable( GL_TEXTURE_2D );

    for( i = 0; i < NB_ATOMS; i++ )
    {
        if( fps )
        {
            atoms[i][0] += 40.0f / fps;
            atoms[i][4] += 30.0f / fps;
        }

        t = atoms[i][0];

        x = t * atoms[i][1];
        y = t * atoms[i][2];
        z = t * atoms[i][3] - 200.0f;

        glColor4f( atoms[i][5], atoms[i][6],
                   atoms[i][7], 1.0f + z / 100.0f );

        glPushMatrix();

        glRotatef( atoms[i][4], 0.0f, 0.0f, 1.0f );

        glBegin( GL_QUADS );

        glTexCoord2f( 1.0f, 1.0f ); glVertex3f(  0.4f + x,  0.4f + y, z );
        glTexCoord2f( 0.0f, 1.0f ); glVertex3f( -0.4f + x,  0.4f + y, z );
        glTexCoord2d( 0.0f, 0.0f ); glVertex3f( -0.4f + x, -0.4f + y, z );
        glTexCoord2d( 1.0f, 0.0f ); glVertex3f(  0.4f + x, -0.4f + y, z );

        glEnd();

        glPopMatrix();

        if( z >= 0.0f )
        {
            CREATE_PARTICLE( atoms[i] );
        }
    }

    glDisable( GL_TEXTURE_2D );

    if( fps && info )
    {
        gl_printf( 1.0f, 1.0f, 1.0f, 0.5f, width - 114, height - 40,
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
    gluPerspective( 50.0, (GLdouble) width / height, 0.1, 100.0 );

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
