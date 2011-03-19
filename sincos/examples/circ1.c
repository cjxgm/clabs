/*
    CIRCLE 1
    Written by Amarillion (amarillion@yahoo.com)

    This program demonstrates the use of sin and cos with floats.
    All it does is draw a circle on the screen.
*/

#include <allegro.h>
#include <math.h>

// Make sure PI is defined.
// MinGW has some problems with this.
#ifndef PI
#define PI 3.1415927
#endif

#define RAD(deg) (deg * PI / 180)

void draw_circle ()
{
    int x, y;
    int length = 50;
    float angle = 0.0;
    float angle_stepsize = RAD(0.5);

    // go through all angles from 0 to 2 * PI radians
    while (angle < RAD(360))
    {
        // calculate x, y from a vector with known length and angle
        x = length * cos (angle);
        y = length * sin (angle);

        putpixel (screen,
            x + SCREEN_W / 2, y + SCREEN_H / 2,
            makecol (255, 255, 255));
        angle += angle_stepsize;
    }
}

int main ()
{
    // initialize Allegro
    if (allegro_init () < 0)
    {
        allegro_message ("Error: Could not initialize Allegro");
        return -1;
    }
    // initialize gfx mode
    if (set_gfx_mode (GFX_AUTODETECT_WINDOWED, 320, 200, 0, 0) < 0)
    {
        allegro_message ("Error: Could not set graphics mode");
        return -1;
    }
    // initialize keyboard
    install_keyboard ();
    clear_keybuf ();

    // call the example function
    draw_circle ();

    // wait for a user key-press
    readkey ();

    // exit Allegro
    allegro_exit ();

    return 0;

} END_OF_MAIN ();
