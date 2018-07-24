/*
 * shell.h
 *
 *  Created on: Dec 30, 2016
 *      Author: wouter
 */

#ifndef SYSTEM_INCLUDE_KERNEL_IO_VGA_H_
#define SYSTEM_INCLUDE_KERNEL_IO_VGA_H_

#include <stdint.h>
#include <stddef.h>

#include <string.h>

enum vga_color {
	BLACK = 0,
	BLUE = 1,
	GREEN = 2,
	CYAN = 3,
	RED = 4,
	MAGENTA = 5,
	BROWN = 6,
	LIGHT_GREY = 7,
	DARK_GREY = 8,
	LIGHT_BLUE = 9,
	LIGHT_GREEN = 10,
	LIGHT_CYAN = 11,
	LIGHT_RED = 12,
	LIGHT_MAGENTA = 13,
	LIGHT_BROWN = 14,
	WHITE = 15,
};

void vga_init(uint32_t *kernel_start, uint32_t *kernel_end);

void vga_putentryat(unsigned char c, enum vga_color foregroundcolor, enum vga_color backgroundcolor, size_t x, size_t y);

void vga_putcharat(char c, size_t x, size_t y);
void vga_putchar(char c);
void vga_write(const char* data, size_t size);
void vga_writeString(const char* data);

void vga_scroll(size_t rows);

void vga_setCursor(size_t x, size_t y);

void vga_setColor(enum vga_color fg, enum vga_color bg);
void vga_setBackgroundColor(enum vga_color color);
void vga_setForegroundColor(enum vga_color color);
enum vga_color vga_getBackgroundColor();
enum vga_color vga_getForegroundColor();

#endif /* SYSTEM_INCLUDE_KERNEL_IO_VGA_H_ */
