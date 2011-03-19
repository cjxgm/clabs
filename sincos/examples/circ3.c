/*
    CIRCLE 3
    Written by Amarillion (amarillion@yahoo.com)

    This example shows a graphical plot of the sine function.
    Note that the plot will appear upside down. This is because
    the y-coordinates are upside down - that is, a higher
    y-coordinate represents a point lower down on the screen.
*/

#include <allegro.h>

void draw_sine ()
{
    int length = 50;
    fixed x, y;
    fixed angle = 0;
    fixed angle_stepsize = itofix (5);

    while (fixtoi(angle) < 256)
    {
        // the angle is plotted along the x-axis
        x = angle;
        // the sine function is plotted along the y-axis
        y = length * fsin (angle);

        putpixel (screen,
            fixtoi (x), fixtoi (y) + SCREEN_H / 2,
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
    draw_sine ();

    // wait for a user key-press
    readkey ();

    // exit Allegro
    allegro_exit ();

    return 0;

} END_OF_MAIN ();
