/*
 * vga.cpp
 *
 *  Created on: Dec 19, 2016
 *      Author: wouter
 */

#include <kernel/drivers/vga.h>

using namespace driver_vga;

/**
 * TODO: Write comment
 */
void driver_vga::vga_initialize() {
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			vga_buffer[index] = vga_entry(' ', vga_entry_color(foregroundcolor, backgroundcolor));
		}
	}
}

/**
 *
 * TODO: Write doc
 *
 * @param c
 * @param foregroundcolor
 * @param backroundcolor
 * @param x
 * @param y
 */
void driver_vga::vga_putentryat(unsigned char c, enum vga_color foregroundcolor, enum vga_color backroundcolor, size_t x, size_t y) {
	const size_t index = y * VGA_WIDTH + x;
	vga_buffer[index] = vga_entry(c, vga_entry_color(foregroundcolor, backroundcolor));
}

/**
 *
 * TODO: Write Doc
 *
 * @param c
 * @param x
 * @param y
 */
void driver_vga::vga_putchar_at(char c, size_t x, size_t y) {
	if(y < VGA_HEIGHT && x < VGA_WIDTH){
		vga_putentryat((unsigned char) c, foregroundcolor, backgroundcolor, x, y);
	}
}

/**
 *
 * TODO: Write Doc
 *
 * @param c
 */
void driver_vga::vga_putchar(char c) {
	unsigned char uc = c;
	if(c != '\n' && c != '\r' && c != '\0'){
		vga_putentryat(uc, foregroundcolor, backgroundcolor, terminal_colomn, terminal_row);
		if (++terminal_colomn == driver_vga::VGA_WIDTH) {
			terminal_colomn = 0;
			if (++terminal_row == driver_vga::VGA_HEIGHT){
				terminal_row = 0;
			}
		}
	} else if(c == '\n' || c == '\r'){
		terminal_colomn = 0;
		terminal_row++;
	}
}

/**
 *
 * TODO: Write Doc
 *
 * @param data
 * @param size
 */
void driver_vga::vga_write(const char* data, size_t size) {
	for (size_t i = 0; i < size; i++){
		vga_putchar(data[i]);
	}
}

/**
 *
 * TODO: Write Doc
 *
 * @param data
 */
void driver_vga::vga_writestring(const char* data) {
	vga_write(data, strlen(data));
}

/**
 *
 * TODO: Write Doc
 *
 * @param x
 * @param y
 */
void driver_vga::vga_setCursor(size_t x, size_t y){
	if(y < VGA_HEIGHT && x < VGA_WIDTH){
		terminal_colomn = x;
		terminal_row = y;
	}
}

/**
 *
 * TODO: Write Doc
 *
 * @param fg
 * @param bg
 */
void driver_vga::vga_setColor(enum driver_vga::vga_color fg, enum driver_vga::vga_color bg){
	foregroundcolor = fg;
	backgroundcolor = bg;
}

/**
 *
 * TODO: Write Doc
 *
 * @param color
 */
void driver_vga::vga_setBackgroundColor(enum driver_vga::vga_color color){
	backgroundcolor = color;
}

/**
 *
 * TODO: Write Doc
 *
 * @param color
 */
void driver_vga::vga_setForegroundColor(enum driver_vga::vga_color color){
	foregroundcolor = color;
}

/**
 *
 * TODO: Write Doc
 *
 * @return
 */
enum driver_vga::vga_color driver_vga::vga_getBackgroundColor(){
	return backgroundcolor;
}

/**
 *
 * TODO: Write Doc
 *
 * @return
 */
enum driver_vga::vga_color driver_vga::vga_getForegroundColor(){
	return foregroundcolor;
}
