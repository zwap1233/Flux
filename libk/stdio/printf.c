#include <limits.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void convertToString(const int num, char *str, const int base, const int len);
void convertUnsignedToString(const unsigned int num, char *str, const int base, const int len);
//void convertDoubleToString(const double num, char *str, const int len);

/**
 * Converts a number to its equivalent with base 'base'
 * @param num: the integer to be converted
 * @param ptr: the string where the number should be written to
 * @param base: the base for the converted number
 */
void convertToString(const int num, char *str, const int base, const int len){
    static char Representation[]= "0123456789ABCDEF";
    char buf[100];
    unsigned int a = num;

    buf[99] = '\0';
    char *ptr = &buf[99];

    if(base < 2 || base > 16){
    	return;
    }

    if(num < 0){
		*str = '-';
		str++;

		a = num*-1;
	}

    do {
    	*--ptr = Representation[a%base];

        a /= base;
    } while(a != 0 && ptr != buf);

    int i;
    for(i = 0; ptr != &buf[99] && i < len-2; ptr++, str++, i++){
    	*str = *ptr;
    }

    if(i < len-1){
    	*str = '\0';
    }
}

/**
 * Converts a number to its equivalent with base 'base'
 * @param num: the integer to be converted
 * @param ptr: the string where the number should be written to
 * @param base: the base for the converted number
 */
void convertUnsignedToString(const unsigned int num, char *str, const int base, const int len){
	static char Representation[]= "0123456789ABCDEF";
	char buf[100];

	buf[99] = '\0';
	char *ptr = &buf[99];

	unsigned int a = num;

	if(base < 2 || base > 16){
		return;
	}

	do {
		*--ptr = Representation[a%base];

		a /= base;
	} while(a != 0 && ptr != buf);

	int i;
	for(i = 0; ptr != &buf[99] && i < len-1; ptr++, str++, i++){
		*str = *ptr;
	}

	if(i < len){
		*str = '\0';
	}
}

/**
 * Converts a double to a string
 * @param num: the double to be converted
 * @param ptr: the string where the number should be written to
 *
void convertDoubleToString(const double num, char *str, const int len){
	convertToString((int) num, str, 10, len);

	char *ptr = str;

	int length_int = strlen(str);

	str += length_int;
	*str = '.';
	str++;

	double a = num;

	if(num < 0){
		a = num*-1.0;
	}

	int b = (int) a;

	double res = a - (int) a;

	int i;
	for(i = 0; a - b != 0.0; i++, a *= 10, res *= 10){
		b = (int) a;
	}

	if((int) res < (int) (res + 0.5)){
		res = (int) res + 1;
	}

	char buf[100];
	convertToString((int) res, buf, 10, len - length_int);

	int d;
	for(d = strlen(buf); d < i; d++){
		*str = '0';
		str++;
	}

	for(i = 0; i < (len - length_int - (int) strlen(buf)) && i < 99; i++){
		str[i] = buf[i];
	}
}*/

static int print(const char* data, size_t length) {
	const unsigned char* bytes = (const unsigned char*) data;

	for (size_t i = 0; i < length; i++){
		if (putchar(bytes[i]) == EOF){
			return 0;
		}
	}

	return 1;
}

int printf(const char* format, ...) {
	va_list parameters;
	va_start(parameters, format);

	int written = 0;

	while(*format != '\0'){
		int max_remaining = INT_MAX - written;

		if (format[0] != '%' || format[1] == '%') {
			if (format[0] == '%'){
				format++;
			}

			int format_len = 1;

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

		const char* format_begun_at = format++;

		int zero_padding = 0;
		int prefix_enable = 0;
		int min_length = 0;


		if(*format == '#'){
			prefix_enable = 1;
			format++;
		}

		if(*format == '0'){
			zero_padding = 1;
			format++;
		}

		if(*format > '0' && *format <= '9'){
			min_length = atoi(format);

			for(; *format >= '0' && *format <= '9'; format++);
		}

		/*
		if(*format == '.'){
			format++;
			if(*format > '0' && *format <= '9'){
				min_decimals = atoi(format);

				for(; *format >= '0' && *format <= '9'; format++);
			}
		}*/

		if (*format == 'c') {
			format++;
			char c = (char) va_arg(parameters, int /* char promotes to int */);

			if (!max_remaining) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}

			if (!print(&c, sizeof(c))){
				return -1;
			}

			written++;
		} else if (*format == 's') {
			format++;
			const char* str = va_arg(parameters, const char*);
			int len = strlen(str);

			if (max_remaining < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}

			if (!print(str, len)){
				return -1;
			}

			written += len;
		} else if (*format == 'x') {
			format++;
			unsigned int hex = va_arg(parameters, unsigned int);

			char str[99];
			convertUnsignedToString(hex, str, 16, 99);
			int len = strlen(str);

			if(hex != 0 && prefix_enable){
				if(max_remaining >= 2){
					if (!print("0x", 2)){
						return -1;
					}
				} else {
					// TODO: Set errno to EOVERFLOW.
					return -1;
				}

				written += 2;
			}

			max_remaining = INT_MAX - written;

			if(len-1 > min_length){
				int i = 0;
				for(i = 0; i < (min_length - len + 1); i++){
					if(max_remaining >= 1){
						if(zero_padding){
							if (!print("0", 1)){
								return -1;
							}
						} else {
							if (!print(" ", 1)){
								return -1;
							}
						}

						written++;
					} else {
						// TODO: Set errno to EOVERFLOW.
						return -1;
					}
				}
			}

			max_remaining = INT_MAX - written;

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
			int dec = va_arg(parameters, int);

			char str[99];
			convertToString(dec, str, 10, 99);
			int len = strlen(str);

			if(len-1 > min_length){
				int i = 0;
				for(i = 0; i < (min_length - len + 1); i++){
					if(max_remaining >= 1){
						if(zero_padding){
							if (!print("0", 1)){
								return -1;
							}
						} else {
							if (!print(" ", 1)){
								return -1;
							}
						}

						written++;
					} else {
						// TODO: Set errno to EOVERFLOW.
						return -1;
					}
				}
			}

			max_remaining = INT_MAX - written;

			if (max_remaining < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}

			if (!print(str, len)){
				return -1;
			}

			written += len;
		} else if (*format == 'b') {
			format++;
			unsigned int bin = va_arg(parameters, unsigned int);

			char str[99];
			convertUnsignedToString(bin, str, 2, 99);
			int len = strlen(str);

			if(bin != 0 && prefix_enable){
				if(max_remaining >= 2){
					if (!print("0b", 2)){
						return -1;
					}
				} else {
					// TODO: Set errno to EOVERFLOW.
					return -1;
				}

				written += 2;
			}

			max_remaining = INT_MAX - written;

			if(len-1 > min_length){
				int i = 0;
				for(i = 0; i < (min_length - len + 1); i++){
					if(max_remaining >= 1){
						if(zero_padding){
							if (!print("0", 1)){
								return -1;
							}
						} else {
							if (!print(" ", 1)){
								return -1;
							}
						}

						written++;
					} else {
						// TODO: Set errno to EOVERFLOW.
						return -1;
					}
				}
			}

			max_remaining = INT_MAX - written;

			if (max_remaining < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}

			if (!print(str, len)){
				return -1;
			}
			written += len;
		/*} else if (*format == 'f') {
			format++;
			double dec = va_arg(parameters, double);

			char str[99];
			convertDoubleToString(dec, str, 99);
			int len = strlen(str);

			if(len-1 > min_length){
				int i = 0;
				for(i = 0; i < (min_length - len + 1); i++){
					if(max_remaining >= 1){
						if(zero_padding){
							if (!print("0", 1)){
								return -1;
							}
						} else {
							if (!print(" ", 1)){
								return -1;
							}
						}

						written++;
					} else {
						// TODO: Set errno to EOVERFLOW.
						return -1;
					}
				}
			}

			max_remaining = INT_MAX - written;

			if (max_remaining < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}

			if (!print(str, len)){
				return -1;
			}

			written += len;*/
		} else {
			format = format_begun_at;
			int len = strlen(format);

			if (max_remaining < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}

			if (!print(format, len)){
				return -1;
			}

			written += len;
			format += len;
		}
	}

	va_end(parameters);
	return written;
}
