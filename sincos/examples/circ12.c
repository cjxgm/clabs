/*
    CIRCLE 12
    Written by Amarillion (amarillion@yahoo.com)

    This program uses "Mode 7" to map a bitmap on to a horizontal plane.
    The mode_7 function is actually pretty similar to the rotate_sprite
    function.

    This is almost the same as CIRCLE 11, but this example also shows
    how to draw objects on the surface of the map.

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
    fixed obj_scale_x, obj_scale_y; // this determines the relative size of
    // the objects
} MODE_7_PARAMS;

/* draw_object just draws a single object at a fixed position, although
this can easily be modified to allow for more objects.
bmp = bitmap to draw to. obj = sprite for the object.
angle, cx, cy define the camera position.
*/
void draw_object (BITMAP *bmp, BITMAP *obj, fixed angle, fixed cx, fixed cy, MODE_7_PARAMS params)
{
    int width, height;
    int screen_y, screen_x;

    // The object in this case is at a fixed position of (160, 100).
    // Calculate the position relative to the camera.
    fixed obj_x = itofix(160) - cx;
    fixed obj_y = itofix(100) - cy;

    // use a rotation transformation to rotate the object by the camera
    // angle
    fixed space_x = fmul (obj_x, fcos (angle)) + fmul (obj_y, fsin (angle));
    fixed space_y = -fmul (obj_x, fsin (angle)) + fmul (obj_y, fcos (angle));

    // calculate the screen coordinates that go with these space coordinates
    // by dividing everything by space_x (the distance)
    screen_x = bmp->w/2 + fixtoi (fmul (fdiv (params.scale_x, space_x), space_y));
    screen_y = fixtoi (fdiv (fmul (params.space_z, params.scale_y), space_x)) - params.horizon;

    // the size of the object has to be scaled according to the distance
    height = fixtoi (obj->h * fdiv(params.obj_scale_y, space_x));
    width = fixtoi (obj->w * fdiv(params.obj_scale_x, space_x));

    // draw the object
    stretch_sprite (bmp, obj, screen_x - width / 2, screen_y - height, width, height);
}

void mode_7 (BITMAP *bmp, BITMAP *tile, fixed angle, fixed cx, fixed cy, MODE_7_PARAMS params)
{
    // current screen position
    int screen_x, screen_y;

    // the distance and horizontal scale of the line we are drawing
    fixed distance, horizontal_scale;

    // masks to make sure we don't read pixels outside the tile
    int mask_x = (tile->w - 1);
    int mask_y = (tile->h -1);

    // step for points in space between two pixels on a horizontal line
    fixed line_dx, line_dy;

    // current space position
    fixed space_x, space_y;

    for (screen_y = 0; screen_y < bmp->h; screen_y++)
    {
        // first calculate the distance of the line we are drawing
        distance = fdiv (fmul (params.space_z, params.scale_y),
            itofix (screen_y + params.horizon));
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
                    fixtoi (space_y) & mask_y ));
            // advance to the next position in space
            space_x += line_dx;
            space_y += line_dy;
        }
    }
}

void test_mode_7 ()
{
    MODE_7_PARAMS params;
    BITMAP *tile, *sprite, *buffer;
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
    params.obj_scale_x = ftofix (50.0);
    params.obj_scale_y = ftofix (50.0);
    params.horizon = 20;

    // to avoid flicker the program makes use of a double-buffering system
    buffer = create_bitmap (screen->w, screen->h);
    // create a 64x64 tile bitmap and draw something on it.
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

    // Create another bitmap and draw something to it.
    // This bitmap contains the object.
    sprite = create_bitmap (64, 64);
    clear (sprite);
    for (i = 0; i < 64; i++)
    {
        for (j = 0; j < 64; j++)
        {
            r2 = (32 - i) * (32 - i) + (32 - j) * (32 - j);
            if (r2 < 30 * 30)
            {
                r2 = (24 - i) * (24 - i) + (24 - j) * (24 - j);
                putpixel (sprite, i, j, 127 - fixtoi(fsqrt(itofix(r2))));
            }
        }
    }

    // create a palette
    // colors for the tiles
    for (i = 0; i < 64; i++)
    {
        pal[i].r = i;
        pal[i].g = i;
        pal[i].b = 0;
    }
    // colors for the object
    for (i = 0; i < 32; i++)
    {
        pal[i+64].r = 0;
        pal[i+64].g = 0;
        pal[i+64].b = 2 * i;
        pal[i+96].r = 2 * i;
        pal[i+96].g = 2 * i;
        pal[i+96].b = 63;
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
        draw_object (buffer, sprite, angle, x, y, params);
        vsync();
        blit (buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

    }
    destroy_bitmap (tile);
    destroy_bitmap (sprite);
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
