#ifndef ARCH_I386_VGA_H
#define ARCH_I386_VGA_H

#include <stdint.h>
#include <stddef.h>
#include <string.h>

namespace driver_vga {

	extern "C"{

		enum vga_color {
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

		static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
			return fg | bg << 4;
		}

		static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
			return (uint16_t) uc | (uint16_t) color << 8;
		}

		static const size_t VGA_WIDTH = 80;
		static const size_t VGA_HEIGHT = 25;
		static const uint16_t* VGA_MEMORY = (uint16_t*) 0xB8000;

		static uint16_t* vga_buffer = (uint16_t *) VGA_MEMORY;

		static volatile enum driver_vga::vga_color foregroundcolor = VGA_COLOR_LIGHT_GREY;
		static volatile enum driver_vga::vga_color backgroundcolor = VGA_COLOR_BLACK;

		static volatile int terminal_colomn = 0;
		static volatile int terminal_row = 0;

		void vga_initialize();

		void vga_putentryat(unsigned char c, enum vga_color foregroundcolor, enum vga_color backgroundcolor, size_t x, size_t y);

		void vga_putchar_at(char c, size_t x, size_t y);
		void vga_putchar(char c);
		void vga_write(const char* data, size_t size);
		void vga_writestring(const char* data);

		void vga_setCursor(size_t x, size_t y);

		void vga_setColor(enum driver_vga::vga_color fg, enum driver_vga::vga_color bg);
		void vga_setBackgroundColor(enum driver_vga::vga_color color);
		void vga_setForegroundColor(enum driver_vga::vga_color color);
		enum driver_vga::vga_color vga_getBackgroundColor();
		enum driver_vga::vga_color vga_getForegroundColor();
	}
}

#endif
