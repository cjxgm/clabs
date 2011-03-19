/*
    CIRCLE 6
    Written by Amarillion (amarillion@yahoo.com)

    This program demonstrates how you can draw a circle without using
    sin or cos. It draws a number of circles on the screen. Circles
    drawn by the function in this example are white, while circles
    drawn by Allegro's circle function are red.
*/

#include <allegro.h>

// my_draw_circle() shows another way of drawing circles.
// center_x and center_y are the center of the circle;
// r is the radius of the circle.
void my_draw_circle (BITMAP *bmp, int center_x, int center_y, int r, int color)
{
    // x and y are the current position in the circle.
    int x = 0, y = r;

    while (x <= y)
    {
        // We make use of 8 axes of symmetry in a circle.
        // This way we have fewer points to calculate on its circumference.
        putpixel (bmp, center_x + x, center_y + y, color);
        putpixel (bmp, center_x - x, center_y + y, color);
        putpixel (bmp, center_x + x, center_y - y, color);
        putpixel (bmp, center_x - x, center_y - y, color);
        putpixel (bmp, center_x + y, center_y + x, color);
        putpixel (bmp, center_x - y, center_y + x, color);
        putpixel (bmp, center_x + y, center_y - x, color);
        putpixel (bmp, center_x - y, center_y - x, color);

        // This is the most important part of the function.
        // We go to the right in all cases (x++).
        // We need to decide whether to go down (y--).
        // This depends on which point is
        // closest to the path of the circle.
        // Good old Pythagoras will tell us what to do.
        x++;
        if (abs (x*x + y*y - r*r) >
            abs (x*x + (y-1)*(y-1) - r*r))
            y--;
    }
}

// just a test function to demonstrate my_draw_circle
void test_draw_circle ()
{
    int i = 1;
    int xpos = 2;
    while (i + i + xpos < SCREEN_W)
    {
        i++;
        // use our circle routine
        my_draw_circle (screen, xpos + i, 100, i, makecol (255, 255, 255));
        // use Allegro's circle routine
        circle (screen, xpos + i, 150, i, makecol (255, 0, 0));
        xpos += i + i + 2;
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
    test_draw_circle ();

    // wait for a user key-press
    readkey ();

    // exit Allegro
    allegro_exit ();

    return 0;

} END_OF_MAIN ();
