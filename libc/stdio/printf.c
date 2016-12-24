#include <limits.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

static int print(const char* data, size_t length) {
	const unsigned char* bytes = (const unsigned char*) data;
	for (size_t i = 0; i < length; i++)
		if (putchar(bytes[i]) == EOF)
			return 0;
	return 1;
}

int printf(const char* format, ...) {
	va_list parameters;
	va_start(parameters, format);

	int written = 0;

	while (*format != '\0') {
		size_t maxrem = INT_MAX - written;

		if (format[0] != '%' || format[1] == '%') {
			if (format[0] == '%')
				format++;
			size_t amount = 1;
			while (format[amount] && format[amount] != '%')
				amount++;
			if (maxrem < amount) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(format, amount))
				return -1;
			format += amount;
			written += amount;
			continue;
		}

		const char* format_begun_at = format++;

		if (*format == 'c') {
			format++;
			char c = (char) va_arg(parameters, int /* char promotes to int */);
			if (!maxrem) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(&c, sizeof(c)))
				return -1;
			written++;
		} else if (*format == 's') {
			format++;
			const char* str = va_arg(parameters, const char*);
			size_t len = strlen(str);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(str, len))
				return -1;
			written += len;
		} else if (*format == 'x') {
			format++;
			uint32_t hex = va_arg(parameters, uint32_t);
			if (maxrem < 10) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}

			char str[10];
			str[0] = '0';
			str[1] = 'x';

			//uint32_t mask = 0xf0000000;
			int i = 0;
			for(i = 0; i < 8; i++){
				uint8_t res = (uint8_t) ((hex >> ((7 - i)*4)) & 0xf);

				if(res > 9){
					str[i + 2] = (char) ((res - 10) + (uint8_t) 'a');
				} else {
					str[i + 2] = (char) (res + (uint8_t) '0');
				}
			}

			print(str, 10);

			written += 10;
		} else {
			format = format_begun_at;
			size_t len = strlen(format);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(format, len))
				return -1;
			written += len;
			format += len;
		}
	}

	va_end(parameters);
	return written;
}
