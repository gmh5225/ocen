#include <gdt.h>
#include <root.h>
#include <idt.h>
#include <rendr/vga>

struct vga_initialize {
	void set_background_color(uint8_t color) {
		for(int i = 0; i < 80 * 25; ++i) {
			vga_buffer[i] = (vga_buffer[i] & 0xFF | (color << 8);

		}
	}
}

void bc() {
	vga_initialize run;
	run.set_background_color(VGA_BLACK);
}
