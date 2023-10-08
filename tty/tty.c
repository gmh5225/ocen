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


/*
 * Writes to a TTY specified by `tty`.
 *
 * @buf: Buffer to write.
 * @len: Length of buffer.
 *
 * Returns number of bytes written, and
 * EXIT_FAILURE on error.
 */
ssize_t
tty_write(struct tty *tty, const char *buf, size_t len)
{
    if (len == 0) {
        /* Bad value, don't even try */
        return EXIT_FAILURE;
    }

    TTY_LOCK(tty);
    for (size_t i = 0; i < len; ++i) {
        tty_push_char(tty, buf[i]);
        /*
         * If we have a newline and we are
         * buffering bytes, flush the ring.
         */
        if (buf[i] == '\n' && __TEST(tty->t_oflag, ORBUF)) {
            tty_flush(tty);
        }
    }

    /*
     * If we aren't buffering bytes, don't
     * keep the bytes within the ring and
     * flush it right away per `tty_write()`
     * call.
     */
    if (!__TEST(tty->t_oflag, ORBUF)) {
        tty_flush(tty);
    }

    TTY_UNLOCK(tty);
    return len;
}

tty_scroll_single(struct tty *tty)
{
    uint32_t *fb_ptr;
    size_t line_size, dest_idx, src_idx;

    fb_ptr = tty->fbdev.mem;
    line_size = tty->fbdev.pitch/4;

    /* Copy each line up */
    for (size_t y = FONT_HEIGHT; y < tty->t_ws_ypixel; y += FONT_HEIGHT) {
        dest_idx = fbdev_get_index(&tty->fbdev, 0, y - FONT_HEIGHT);
        src_idx = fbdev_get_index(&tty->fbdev, 0, y);
        memcpy32(&fb_ptr[dest_idx], &fb_ptr[src_idx], FONT_HEIGHT*line_size);
    }

    /*
     * Ensure we start at X position 0
     * after we scrolled down.
     */
    tty->chpos_x = 0;
    tty->curspos_x = 0;
}
