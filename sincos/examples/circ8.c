/*
    CIRCLE 8
    Written by Amarillion (amarillion@yahoo.com)

    This example shows how you can use the dot product to determine
    in which direction your homing missile should go.
    The homing missile in this case is actually a circle with a
    red line representing the direction. Each time the
    missile reaches its target, a new target is set.
*/

#include <allegro.h>

void dot_product_home_in ()
{
    // the position of the homing missile
    fixed x = itofix(SCREEN_W / 2);
    fixed y = itofix(SCREEN_H / 2);
    // the angle and length of the missile's velocity vector
    fixed angle = 0;
    int length = 1;
    fixed angle_stepsize = itofix (3);
    // determines whether the missile has reached
    // the target and a new one should be chosen
    int new_target = TRUE;
    // position of the target
    fixed target_x, target_y;
    // vector of missile movement
    fixed dx, dy;

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

        // Move the missile
        // We store dx and dy in variables so that
        // we can use them later on in the dot product.
        dx = length * fcos (angle);
        dy = length * fsin (angle);
        x += dx;
        y += dy;

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

        // Determine whether we should turn left or right
        // using the dot product.
        // We use & 0xFFFFFF as a trick to get an angle
        // between 0 and 256.
        if (fmul(dy,(target_x - x)) + fmul(-dx,(target_y - y)) > 0)
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
    dot_product_home_in ();

    // wait for a user key-press
    readkey ();

    // exit Allegro
    allegro_exit ();

    return 0;

} END_OF_MAIN ();
