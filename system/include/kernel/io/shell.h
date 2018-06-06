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

		void initialize(uint32_t *kernel_start, uint32_t *kernel_end);

		void putentryat(unsigned char c, enum shell::shell_color foregroundcolor, enum shell::shell_color backgroundcolor, size_t x, size_t y);

		void putcharat(char c, size_t x, size_t y);
		void putchar(char c);
		void write(const char* data, size_t size);
		void writeString(const char* data);

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
