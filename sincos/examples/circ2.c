/*
    CIRCLE 2
    Written by Amarillion (amarillion@yahoo.com)

    This example is almost the same as CIRCLE 1, only this works with
    fixed-point mathematics instead of floating-point.
*/

#include <allegro.h>

void draw_circle_fixed ()
{
    fixed x, y;
    int length = 50;
    fixed angle = 0;
    fixed angle_stepsize = itofix (5);

    // go through all angles from 0 to 255
    while (fixtoi (angle) < 256)
    {
        // calculate x, y from a vector with known length and angle
        x = length * fcos (angle);
        y = length * fsin (angle);

        putpixel (screen,
            fixtoi(x) + SCREEN_W / 2, fixtoi(y) + SCREEN_H / 2,
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
    draw_circle_fixed ();

    // wait for a user key-press
    readkey ();

    // exit Allegro
    allegro_exit ();

    return 0;

} END_OF_MAIN ();
