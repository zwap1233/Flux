/*
 * shell.h
 *
 *  Created on: Dec 30, 2016
 *      Author: wouter
 */

#ifndef SYSTEM_INCLUDE_KERNEL_SHELL_H_
#define SYSTEM_INCLUDE_KERNEL_SHELL_H_

#include <stdint.h>
#include <stddef.h>

#include <string.h>

namespace shell {

	extern "C"{

		enum shell_color {
			VGA_COLOR_BLACK = 0,
			VGA_COLOR_BLUE = 1,
			VGA_COLOR_GREEN = 2,
			VGA_COLOR_CYAN = 3,
			VGA_COLOR_RED = 4,
			VGA_COLOR_MAGENTA = 5,
			VGA_COLOR_BROWN = 6,
			VGA_COLOR_LIGHT_GREY = 7,
			VGA_COLOR_DARK_GREY = 8,
			VGA_COLOR_LIGHT_BLUE = 9,
			VGA_COLOR_LIGHT_GREEN = 10,
			VGA_COLOR_LIGHT_CYAN = 11,
			VGA_COLOR_LIGHT_RED = 12,
			VGA_COLOR_LIGHT_MAGENTA = 13,
			VGA_COLOR_LIGHT_BROWN = 14,
			VGA_COLOR_WHITE = 15,
		};

		static inline uint8_t vga_entry_color(enum shell_color fg, enum shell_color bg) {
			return (uint8_t) fg | bg << 4;
		}

		static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
			return (uint16_t) uc | (uint16_t) color << 8;
		}

		static const size_t VGA_WIDTH = 80;
		static const size_t VGA_HEIGHT = 25;
		static const uint16_t* VGA_MEMORY = (uint16_t*) 0xC00B8000;

		static uint16_t* vga_buffer = (uint16_t *) VGA_MEMORY;	//offset terminal by one line

		static volatile enum shell::shell_color foregroundcolor = VGA_COLOR_LIGHT_GREY;
		static volatile enum shell::shell_color backgroundcolor = VGA_COLOR_BLACK;

		static volatile int shell_x = 0;
		static volatile int shell_y = 0;

		static const int SHELL_WIDTH = VGA_WIDTH;
		static const int SHELL_HEIGHT = VGA_HEIGHT - 2;

		void initialize(uint32_t *kernel_start, uint32_t *kernel_end);

		void putentryat(unsigned char c, enum shell::shell_color foregroundcolor, enum shell::shell_color backgroundcolor, size_t x, size_t y);

		void putchar_at(char c, size_t x, size_t y);
		void putchar(char c);
		void write(const char* data, size_t size);
		void writestring(const char* data);

		void scroll(size_t rows);

		void setCursor(size_t x, size_t y);

		void setColor(enum shell::shell_color fg, enum shell::shell_color bg);
		void setBackgroundColor(enum shell::shell_color color);
		void setForegroundColor(enum shell::shell_color color);
		enum shell::shell_color getBackgroundColor();
		enum shell::shell_color getForegroundColor();
	}
}

#endif /* SYSTEM_INCLUDE_KERNEL_SHELL_H_ */
