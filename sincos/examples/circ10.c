/*
    CIRCLE 10
    Written by Amarillion (amarillion@yahoo.com)

    This example shows how you can rotate a point around another
    point when you only know the x- and y-coordinates of the points.
*/

#include <allegro.h>

void projection_test()
{
    // initialize the coordinates of four dots
    fixed dot_x[4] = {itofix(-50), itofix(-50), itofix(50), itofix(50)};
    fixed dot_y[4] = {itofix(-50), itofix(50), itofix(50), itofix(-50)};

    fixed angle = 0;
    fixed angle_stepsize = itofix (1);

    // proj_x and proj_y will contain the projection of the dots
    fixed proj_x[4];
    fixed proj_y[4];

    int i;

    // repeat this loop until Esc is pressed
    while (!key[KEY_ESC])
    {
        // project all the dots to their new positions after rotation
        for (i = 0; i < 4; i++)
        {
            proj_x[i] = fmul (dot_x[i], fcos (angle)) -
                fmul (dot_y[i], fsin (angle));
            proj_y[i] = fmul (dot_x[i], fsin (angle)) +
                fmul (dot_y[i], fcos (angle));
        }

        // draw the four dots
        for (i = 0; i < 4; i++)
        {
            putpixel (screen,
                fixtoi (proj_x[i]) + SCREEN_W / 2,
                fixtoi (proj_y[i]) + SCREEN_H / 2,
                makecol (255 ,255, 255));
        }

        rest (10);
        clear (screen);

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

    install_timer ();

    // call the example function
    projection_test ();

    // exit Allegro
    allegro_exit ();

    return 0;

} END_OF_MAIN ();
