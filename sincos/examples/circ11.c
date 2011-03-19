/*
    CIRCLE 11
    Written by Amarillion (amarillion@yahoo.com)

    This program uses "Mode 7" to map a bitmap on to a horizontal plane.
    The mode_7 function is actually pretty similar to the rotate_sprite
    function.

    You can move around with the up, down, left and right keys. Use the
    following keys to change the Mode 7 parameters:
    H / J : move the horizon up / down
    Q / W : change the x scale
    E / R : change the y scale
    Z / X : change the camera height
*/

#include <allegro.h>

/* MODE_7_PARAMS is a struct containing all the different parameters
that are relevant for Mode 7, so you can pass them to the functions
as a single unit */
typedef struct MODE_7_PARAMS
{
    fixed space_z; // this is the height of the camera above the plane
    int horizon; // this is the number of pixels line 0 is below the horizon
    fixed scale_x, scale_y; // this determines the scale of space coordinates
    // to screen coordinates
} MODE_7_PARAMS;

void mode_7 (BITMAP *bmp, BITMAP *tile, fixed angle, fixed cx, fixed cy, MODE_7_PARAMS params)
{
    // current screen position
    int screen_x, screen_y;

    // the distance and horizontal scale of the line we are drawing
    fixed distance, horizontal_scale;

    // masks to make sure we don't read pixels outside the tile
    int mask_x = (tile->w - 1);
    int mask_y = (tile->h - 1);

    // step for points in space between two pixels on a horizontal line
    fixed line_dx, line_dy;

    // current space position
    fixed space_x, space_y;

    for (screen_y = 0; screen_y < bmp->h; screen_y++)
    {
        // first calculate the distance of the line we are drawing
        distance = fmul (params.space_z, params.scale_y) /
            (screen_y + params.horizon);
        // then calculate the horizontal scale, or the distance between
        // space points on this horizontal line
        horizontal_scale = fdiv (distance, params.scale_x);

        // calculate the dx and dy of points in space when we step
        // through all points on this line
        line_dx = fmul (-fsin(angle), horizontal_scale);
        line_dy = fmul (fcos(angle), horizontal_scale);

        // calculate the starting position
        space_x = cx + fmul (distance, fcos(angle)) - bmp->w/2 * line_dx;
        space_y = cy + fmul (distance, fsin(angle)) - bmp->w/2 * line_dy;

        // go through all points in this screen line
        for (screen_x = 0; screen_x < bmp->w; screen_x++)
        {
            // get a pixel from the tile and put it on the screen
            putpixel (bmp, screen_x, screen_y,
                getpixel (tile,
                    fixtoi (space_x) & mask_x,
                    fixtoi (space_y) & mask_y));
            // advance to the next position in space
            space_x += line_dx;
            space_y += line_dy;
        }
    }
}

void test_mode_7 ()
{
    MODE_7_PARAMS params;
    BITMAP *tile, *buffer;
    PALETTE pal;
    int quit = FALSE;
    fixed angle = itofix (0);
    fixed x = 0, y = 0;
    fixed dx = 0, dy = 0;
    fixed speed = 0;
    int i, j, r2;

    params.space_z = itofix (50);
    params.scale_x = ftofix (200.0);
    params.scale_y = ftofix (200.0);
    params.horizon = 20;

    // to avoid flicker the program makes use of a double-buffering system
    buffer = create_bitmap (screen->w, screen->h);
    // create a 64x64 tile bitmap and draw something on it
    tile = create_bitmap (64, 64);
    for (i = 0; i < 32; i++)
    {
        for (j = i; j < 32; j++)
        {
            putpixel (tile, i, j, i);
            putpixel (tile, i, 63-j, i);
            putpixel (tile, 63-i, j, i);
            putpixel (tile, 63-i, 63-j, i);
            putpixel (tile, j, i, i);
            putpixel (tile, j, 63-i, i);
            putpixel (tile, 63-j, i, i);
            putpixel (tile, 63-j, 63-i, i);
        }
    }
    text_mode (-1);

    // create a palette
    for (i = 0; i < 64; i++)
    {
        pal[i].r = i;
        pal[i].g = i;
        pal[i].b = 0;
    }
    set_palette (pal);

    while (!quit)
    {
        // act on keyboard input
        if (key[KEY_ESC]) quit = TRUE;
        if (key[KEY_UP] && speed < itofix (5))
            speed += ftofix (0.1);
        if (key[KEY_DOWN] && speed > itofix (-5))
            speed -= ftofix (0.1);
        if (key[KEY_LEFT])
            angle = (angle - itofix (3)) & 0xFFFFFF;
        if (key[KEY_RIGHT])
            angle = (angle + itofix (3)) & 0xFFFFFF;
        if (key[KEY_Z])
            params.space_z += itofix(5);
        if (key[KEY_X])
            params.space_z -= itofix(5);
        if (key[KEY_Q])
            params.scale_x = fmul (params.scale_x, ftofix (1.5));
        if (key[KEY_W])
            params.scale_x = fdiv (params.scale_x, ftofix (1.5));
        if (key[KEY_E])
            params.scale_y = fmul (params.scale_y, ftofix (1.5));
        if (key[KEY_R])
            params.scale_y = fdiv (params.scale_y, ftofix (1.5));
        if (key[KEY_H])
            params.horizon++;
        if (key[KEY_J])
            params.horizon--;

        dx = fmul (speed, fcos (angle));
        dy = fmul (speed, fsin (angle));

        x += dx;
        y += dy;

        mode_7 (buffer, tile, angle, x, y, params);
        vsync ();
        blit (buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

    }
    destroy_bitmap (tile);
    destroy_bitmap (buffer);
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
    test_mode_7 ();

    // exit Allegro
    allegro_exit ();

    return 0;

} END_OF_MAIN ();
