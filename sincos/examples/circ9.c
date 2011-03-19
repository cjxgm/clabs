/*
    CIRCLE 9
    Written by Amarillion (amarillion@yahoo.com)

    This program demonstrates how a sprite rotation function
    works.
*/

#include <allegro.h>

// my_rotate_sprite will draw src_bmp on to dest_bmp
// rotated by angle degrees and scaled by the scale factor.
// The width and height of the src bitmap must be
// powers of 2 (e.g. 32, 64, 128).
void my_rotate_sprite (BITMAP *dest_bmp, BITMAP *src_bmp,
    fixed angle, fixed scale)
{
    // current position in the source bitmap
    fixed src_x, src_y;

    // current position in the destination bitmap
    int dest_x, dest_y;

    // src_x and src_y will change each time by dx and dy
    fixed dx, dy;

    // src_x and src_y will be initialized to start_x and start_y
    // at the beginning of each new line
    fixed start_x = 0, start_y = 0;

    // We create a bit mask to make sure x and y are in bounds.
    // Unexpected things will happen
    // if the width or height are not powers of 2.
    int x_mask = src_bmp->w - 1;
    int y_mask = src_bmp->h - 1;

    // calculate increments for the coordinates in the source bitmap
    // for when we move right one pixel on the destination bitmap
    dx = fmul (fcos (angle), scale);
    dy = fmul (fsin (angle), scale);

    for (dest_y = 0; dest_y < dest_bmp->h; dest_y++)
    {
        // set the position in the source bitmap to the
        // beginning of this line
        src_x = start_x;
        src_y = start_y;

        for (dest_x = 0; dest_x < dest_bmp->w; dest_x++)
        {
            // Copy a pixel.
            // This can be optimized a lot by using
            // direct bitmap access.
            putpixel (dest_bmp, dest_x, dest_y,
                getpixel (src_bmp,
                    fixtoi (src_x) & x_mask,
                    fixtoi (src_y) & y_mask));

            // advance the position in the source bitmap
            src_x += dx;
            src_y += dy;
        }

        // for the next line we have a different starting position
        start_x -= dy;
        start_y += dx;
    }
}

// This function is just a small demo of my_rotate_sprite
void test_rotate_sprite ()
{
    BITMAP *bmp;
    PALETTE pal;
    fixed angle = 0;
    fixed scale = 0;
    fixed angle_stepsize = itofix (1);

    int i, j;

    // create a bitmap of size 64x64 and fill it with something
    bmp = create_bitmap (64, 64);
    for (i = 0; i < 32; i++)
    {
        for (j = i; j < 32; j++)
        {
            putpixel (bmp, i, j, i);
            putpixel (bmp, i, 63-j, i);
            putpixel (bmp, 63-i, j, i);
            putpixel (bmp, 63-i, 63-j, i);
            putpixel (bmp, j, i, i);
            putpixel (bmp, j, 63-i, i);
            putpixel (bmp, 63-j, i, i);
            putpixel (bmp, 63-j, 63-i, i);
        }
    }

    // create a palette
    for (i = 0; i < 64; i++)
    {
        pal[i].r = i;
        pal[i].g = i;
        pal[i].b = 0;
    }
    set_palette (pal);

    while (!key[KEY_ESC])
    {
        angle += angle_stepsize;
        scale = fsin(angle) + ftofix (1.5);
        my_rotate_sprite (screen, bmp, angle, scale);
        rest (10);
    }
    destroy_bitmap (bmp);
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
    test_rotate_sprite ();

    // exit Allegro
    allegro_exit ();

    return 0;

} END_OF_MAIN ();
