#include <tty/tty.h>

/*
 * Default cursor color.
 *
 * TODO: The cursor color should
 *       be the invert of the color
 *       of whatever it is on top of.
 */

#define DEFAULT_CURSOR_BG 0x808080

static void tty_draw_cursor(struct tty *tty, bool hide)
{
    uint32_t x, y, idx;
    uint32_t *fb_ptr;
    uint32_t color;

    fb_ptr = tty->fbdev.mem;
    color = hide ? tty->bg : DEFAULT_CURSOR_BG;

    for (size_t cy = 0; cy < CURSOR_HEIGHT; ++cy) {
        for (size_t cx = 0; cx < CURSOR_WIDTH; ++cx) {
            x = tty->curspos_x + cx;
            y = tty->curspos_y + cy;
            idx = fbdev_get_index(&tty->fbdev, x, y);
            fb_ptr[idx] = color;
        }
    }
}
