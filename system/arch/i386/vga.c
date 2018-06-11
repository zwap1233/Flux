/*
 * shell.c
 *
 *  Created on: Dec 30, 2016
 *      Author: wouter
 */

#include <stdio.h>
#include <kernel/io/vga.h>

static uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
	return (uint8_t) fg | bg << 4;
}

static uint16_t vga_entry(unsigned char uc, uint8_t color) {
	return (uint16_t) uc | (uint16_t) color << 8;
}

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
static const uint16_t* VGA_MEMORY = (uint16_t*) 0xC00B8000;

uint16_t* term_buffer = 0;	//offset terminal by one line

static volatile enum vga_color foregroundcolor = LIGHT_GREY;
static volatile enum vga_color backgroundcolor = BLACK;

static volatile int cursor_x = 0;
static volatile int cursor_y = 0;

#define TERM_WIDTH VGA_WIDTH
#define TERM_HEIGHT (VGA_HEIGHT - 2)

/**
 * Initialize the shell should be called only once by the kernel after the global constructors
 */
void vga_init(uint32_t *kernel_start, uint32_t *kernel_end) {

	term_buffer = VGA_MEMORY;

	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			term_buffer[index] = vga_entry(' ', vga_entry_color(foregroundcolor, backgroundcolor));
		}
	}

	setForegroundColor(LIGHT_GREEN);
	writeString("FluxOS version 0.0.1");

	cursor_x = 0;
	cursor_y = VGA_HEIGHT - 1;
	printf("Start: %#x                   End: %#x", kernel_start, kernel_end);

	term_buffer = (uint16_t *) VGA_MEMORY + VGA_WIDTH;
	setCursor(0,0);
	setForegroundColor(LIGHT_GREY);
}

/**
 * Place a char with the given properties in the buffer. should not be called to print a char, use putchar_at instead.
 *
 * @param c
 * @param foregroundcolor
 * @param backroundcolor
 * @param x
 * @param y
 */
void vga_putentryat(unsigned char c, enum vga_color fgcolor, enum vga_color bgcolor, size_t x, size_t y) {
	const size_t index = y * VGA_WIDTH + x;
	term_buffer[index] = vga_entry(c, vga_entry_color(fgcolor, bgcolor));
}

/**
 * Write a single char at the given position
 *
 * @param c
 * @param x
 * @param y
 */
void vga_putcharat(char c, size_t x, size_t y) {
	setCursor(x, y);
	putchar(c);
}

/**
 * Write a char at the next possition in the shell
 *
 * @param c
 */
void vga_putchar(char c) {
	unsigned char uc = (unsigned char) c;

	if(c != '\n' && c != '\r' && c != '\0'){
		putentryat(uc, foregroundcolor, backgroundcolor, cursor_x, cursor_y);

		if((cursor_x++) >= TERM_WIDTH){
			cursor_x = 0;
			if((cursor_y++) >= (TERM_HEIGHT - 1)){
				scroll(1);
			}
		}
	} else if(c == '\n' || c == '\r'){
		cursor_x = 0;
		if((cursor_y++) >= (TERM_HEIGHT - 1)){
			scroll(1);
		}
	}
}

/**
 * write an array of chars to the shell
 *
 * @param data
 * @param size
 */
void vga_write(const char* data, size_t size) {
	for (size_t i = 0; i < size; i++){
		putchar(data[i]);
	}
}

/**
 * write a string to the shell
 *
 * @param data
 */
void vga_writeString(const char* data) {
	write(data, strlen(data));
}

/**
 * Shifts the entire terminal up by a given amount of rows
 * changes shell_y accourding to this but not shell_x
 *
 * @param rows
 */
void vga_scroll(size_t rows){
	//TODO: write vga_scroll function body
	cursor_y = 0;
}

/**
 * TODO: Write Doc
 *
 * @param x
 * @param y
 */
void vga_setCursor(size_t x, size_t y){
	if(y < TERM_HEIGHT && x < TERM_WIDTH){
		cursor_y = x;
		cursor_y = y;
	}
}

/**
 * TODO: Write Doc
 *
 * @param fg
 * @param bg
 */
void vga_setColor(enum vga_color fg, enum vga_color bg){
	foregroundcolor = fg;
	backgroundcolor = bg;
}

/**
 * TODO: Write Doc
 *
 * @param color
 */
void vga_setBackgroundColor(enum vga_color color){
	backgroundcolor = color;
}

/**
 * TODO: Write Doc
 *
 * @param color
 */
void vga_setForegroundColor(enum vga_color color){
	foregroundcolor = color;
}

/**
 * TODO: Write Doc
 *
 * @return
 */
enum vga_color vga_getBackgroundColor(){
	return backgroundcolor;
}

/**
 * TODO: Write Doc
 *
 * @return
 */
enum vga_color vga_getForegroundColor(){
	return foregroundcolor;
}


