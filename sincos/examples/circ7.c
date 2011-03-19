/*
    CIRCLE 7
    Written by Amarillion (amarillion@yahoo.com)

    This program shows how you can use atan2 to make a homing missile.
    The homing missile in this case is actually a circle with a
    red line representing the direction. Each time the
    missile reaches its target, a new target is set.
*/

#include <allegro.h>

void home_in ()
{
    // the x, y position of the homing missile
    fixed x = itofix(SCREEN_W / 2);
    fixed y = itofix(SCREEN_H / 2);
    // the angle and length of the missile's velocity vector
    fixed angle = 0;
    int length = 1;
    fixed angle_stepsize = itofix (3);
    // determines whether the missile has reached
    // the target and a new one should be chosen
    int new_target = TRUE;
    // angle to the target
    fixed target_angle;
    // position of the target
    fixed target_x, target_y;

    while (!keypressed())
    {
        clear (screen);
        // choose new target randomly when needed
        if (new_target)
        {
            target_x = itofix((SCREEN_W + rand() % (2 * SCREEN_W)) / 4);
            target_y = itofix((SCREEN_H + rand() % (2 * SCREEN_H)) / 4);
            new_target = FALSE;
        }

        // move the missile
        x += length * fcos (angle);
        y += length * fsin (angle);

        // if we are very close to the target, set a new target
        if (abs (x - target_x) + abs (y - target_y) < itofix(10))
            new_target = TRUE;

        // draw a pixel where the target is
        putpixel (screen, fixtoi(target_x), fixtoi(target_y),
            makecol (255, 255, 255));

        // draw the missile
        // (actually a circle with a line representing the angle)
        circle (screen, fixtoi(x), fixtoi(y), 10, makecol (0, 0, 255));
        line (screen, fixtoi(x), fixtoi(y),
            fixtoi(x) + fixtoi (9 * fcos (angle)),
            fixtoi(y) + fixtoi (9 * fsin (angle)),
            makecol (255, 0, 0));

        // calculate the angle from the missile to the target
        target_angle = fatan2 (target_y - y, target_x - x);

        // Determine whether we should turn left or right.
        // Note that itofix (128) represents half a circle.
        // We use & 0xFFFFFF as a trick to get an angle
        // between 0 and 256.
        if (((angle-target_angle) & 0xFFFFFF) < itofix(128))
            angle = (angle - angle_stepsize) & 0xFFFFFF;
        else
            angle = (angle + angle_stepsize) & 0xFFFFFF;

        rest (10);
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
    home_in ();

    // exit Allegro
    allegro_exit ();

    return 0;

} END_OF_MAIN ();
