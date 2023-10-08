#include <sys/tty.h>
#include <sys/cdefs.h>
#include <sys/errno.h>
#include <sys/ascii.h>
#include <string.h>
#include <sys/tty_font.h>

#include "tty/fetch.h"

#define DEFAULT_CURSOR_BG 0x808080

#define CURSOR_WIDTH            FONT_WIDTH
#define CURSOR_HEIGHT           FONT_HEIGHT
