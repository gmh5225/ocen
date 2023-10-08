#ifndef _RENDR_RENDR_H
#define _RENDR_RENDR_H
#include <stdint.h>

volatile uint16_t* vga_buffer = (uint16_t*)0xB8000

#define VGA_BLACK 0
#define VGA_WHITE 15

void set_background_color(uint8_t* color);

void vga_initialize();
