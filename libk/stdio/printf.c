#include <limits.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

/**
 * Converts a number to its equivalent with base 'base'
 * @param num: the integer to be converted
 * @param base: the base for the converted number
 * @return: a pointer to a char array with the new number
 */
char *convert(int num, int base) {
    static char Representation[]= "0123456789ABCDEF";
    static char buffer[128];
    char *ptr;

    unsigned int a = num;

    ptr = &buffer[127];
    *ptr = '\0';

    if(base < 2 || base > 16){
    	return ptr;
    }

    if(num < 0){
    	a = -1*num;
    }

    do {
    	*--ptr = Representation[a%base];
        a /= base;
    } while(a != 0);

    return(ptr);
}

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

	while(*format != '\0'){
		size_t max_remaining = INT_MAX - written;

		if (format[0] != '%' || format[1] == '%') {
			if (format[0] == '%'){
				format++;
			}

			size_t format_len = 1;

			while (format[format_len] && format[format_len] != '%'){
				format_len++;
			}

			if (max_remaining < format_len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}

			if (!print(format, format_len)){
				return -1;
			}

			format += format_len;
			written += format_len;

			continue;
		}

		int zero_padding = 0;
		int prefix_enable = 0;

		//TODO: Finish implementation
		if(*format == '#'){
			prefix_enable = 1;
			format++;
		} else if(*format == '0'){
			zero_padding = 1;
			format++;
		}

		if(*format >= '0' && *format <= '9'){
			//TODO: Implement
		}

		const char* format_begun_at = format++;

		if (*format == 'c') {
			format++;
			char c = (char) va_arg(parameters, int /* char promotes to int */);
			if (!max_remaining) {
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
			if (max_remaining < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(str, len))
				return -1;
			written += len;
		} else if (*format == 'x') {
			format++;
			unsigned int hex = va_arg(parameters, unsigned int);
			const char *str = convert(hex, 16);
			size_t len = strlen(str);
			if (max_remaining < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}

			if (!print(str, len)){
				return -1;
			}
			written += len;
		} else if (*format == 'd') {
			format++;
			unsigned int dec = va_arg(parameters, unsigned int);
			const char *str = convert(dec, 10);
			size_t len = strlen(str);
			if (max_remaining < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}

			if (!print(str, len)){
				return -1;
			}
			written += len;
		} else if (*format == 'B') {
			format++;
			unsigned int bin = va_arg(parameters, unsigned int);
			const char *str = convert(bin, 2);
			size_t len = strlen(str);
			if (max_remaining < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}

			if (!print(str, len)){
				return -1;
			}
			written += len;
		} else {
			format = format_begun_at;
			size_t len = strlen(format);
			if (max_remaining < len) {
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
