/*
 * shell.c
 *
 *  Created on: Dec 30, 2016
 *      Author: wouter
 */

#include <kernel/io/shell.h>
#include <stdio.h>

using namespace shell;

static uint8_t vga_entry_color(enum shell_color fg, enum shell_color bg) {
	return (uint8_t) fg | bg << 4;
}

static uint16_t vga_entry(unsigned char uc, uint8_t color) {
	return (uint16_t) uc | (uint16_t) color << 8;
}

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
static const uint16_t* VGA_MEMORY = (uint16_t*) 0xC00B8000;

static uint16_t* vga_buffer = (uint16_t *) VGA_MEMORY;	//offset terminal by one line

static volatile enum shell::shell_color foregroundcolor = LIGHT_GREY;
static volatile enum shell::shell_color backgroundcolor = BLACK;

static volatile int shell_x = 0;
static volatile int shell_y = 0;

static const int SHELL_WIDTH = VGA_WIDTH;
static const int SHELL_HEIGHT = VGA_HEIGHT - 2;

/**
 * Initialize the shell should be called only once by the kernel after the global constructors
 */
void shell::initialize(uint32_t *kernel_start, uint32_t *kernel_end) {
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			vga_buffer[index] = vga_entry(' ', vga_entry_color(foregroundcolor, backgroundcolor));
		}
	}

	setForegroundColor(LIGHT_GREEN);
	writeString("FluxOS version 0.0.1");

	shell_x = 0;
	shell_y = VGA_HEIGHT - 1;
	printf("Start: %#x                   End: %#x", kernel_start, kernel_end);

	vga_buffer = (uint16_t *) VGA_MEMORY + VGA_WIDTH;
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
void shell::putentryat(unsigned char c, enum shell_color fgcolor, enum shell_color bgcolor, size_t x, size_t y) {
	const size_t index = y * VGA_WIDTH + x;
	vga_buffer[index] = vga_entry(c, vga_entry_color(fgcolor, bgcolor));
}

/**
 * Write a single char at the given position
 *
 * @param c
 * @param x
 * @param y
 */
void shell::putcharat(char c, size_t x, size_t y) {
	setCursor(x, y);
	putchar(c);
}

/**
 * Write a char at the next possition in the shell
 *
 * @param c
 */
void shell::putchar(char c) {
	unsigned char uc = (unsigned char) c;

	if(c != '\n' && c != '\r' && c != '\0'){
		putentryat(uc, foregroundcolor, backgroundcolor, shell_x, shell_y);

		if((shell_x++) >= SHELL_WIDTH){
			shell_x = 0;
			if((shell_y++) >= (SHELL_HEIGHT - 1)){
				scroll(1);
			}
		}
	} else if(c == '\n' || c == '\r'){
		shell_x = 0;
		if((shell_y++) >= (SHELL_HEIGHT - 1)){
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
void shell::write(const char* data, size_t size) {
	for (size_t i = 0; i < size; i++){
		putchar(data[i]);
	}
}

/**
 * write a string to the shell
 *
 * @param data
 */
void shell::writeString(const char* data) {
	write(data, strlen(data));
}

/**
 * Shifts the entire terminal up by a given amount of rows
 * changes shell_y accourding to this but not shell_x
 *
 * @param rows
 */
void shell::scroll(size_t rows){
	//TODO: write shell::scroll function body
	shell_y = 0;
}

/**
 * TODO: Write Doc
 *
 * @param x
 * @param y
 */
void shell::setCursor(size_t x, size_t y){
	if(y < SHELL_HEIGHT && x < SHELL_WIDTH){
		shell_x = x;
		shell_y = y;
	}
}

/**
 * TODO: Write Doc
 *
 * @param fg
 * @param bg
 */
void shell::setColor(enum shell::shell_color fg, enum shell::shell_color bg){
	foregroundcolor = fg;
	backgroundcolor = bg;
}

/**
 * TODO: Write Doc
 *
 * @param color
 */
void shell::setBackgroundColor(enum shell::shell_color color){
	backgroundcolor = color;
}

/**
 * TODO: Write Doc
 *
 * @param color
 */
void shell::setForegroundColor(enum shell::shell_color color){
	foregroundcolor = color;
}

/**
 * TODO: Write Doc
 *
 * @return
 */
enum shell::shell_color shell::getBackgroundColor(){
	return backgroundcolor;
}

/**
 * TODO: Write Doc
 *
 * @return
 */
enum shell::shell_color shell::getForegroundColor(){
	return foregroundcolor;
}


