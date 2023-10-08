#include <gdt.h>
#include <root.h>
#include <idt.h>
#include <rendr/vga>
#include <sys/tty.h>
#include <sys/ascii.h>
#include <sys/tty_font.h>

#define DEFAULT_CURSOR_BG 0x808080
#define CURSOR_WIDTH            FONT_WIDTH
#define CURSOR_HEIGHT           FONT_HEIGHT


static void
tty_draw_char(struct tty *tty, char c, uint32_t fg, uint32_t bg)
{
    uint32_t *fb_ptr;
    uint32_t x, y;
    size_t idx;
    const uint8_t *glyph;

    /* Get a pointer to framebuffer memory */
    fb_ptr = tty->fbdev.mem;

    /* Get the specific glyph of `c` */
    glyph = &DEFAULT_FONT_DATA[(int)c*16];

    x = tty->chpos_x;
    y = tty->chpos_y;

    for (uint32_t cy = 0; cy < FONT_HEIGHT; ++cy) {
        for (uint32_t cx = 0; cx < FONT_WIDTH; ++cx) {
            idx = fbdev_get_index(&tty->fbdev, x+FONT_WIDTH-cx, y+cy);
            fb_ptr[idx] = __TEST(glyph[cy], __BIT(cx)) ? fg : bg;
        }
    }
}

/*
 * Draws a cursor onto
 * the screen.
 *
 * Call with TTY locked.
 */
static void
tty_draw_cursor(struct tty *tty, bool hide)
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
