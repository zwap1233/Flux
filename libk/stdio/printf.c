#include <limits.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

int convertToNumber(const char *str, int *len, int base);
char *convertToString(int num, int base);

/**
 * Converts a string of numbers with the base 'base' to a single number
 * @param str: the string to be converted
 * @param len: a pointer to a int where the length of the found number is stored in
 * @param base: the base for the converted number
 * @return: a number
 */
int convertToNumber(const char *str, int *len, int base){
	int result = 0;
	int neg = 0;
	int a = 0;

	if(base < 2 || base > 16){
		return 0;
	}

	char cons_a = '0';
	char cons_b = '9';
	char cons_c = 'a';
	char cons_d = 'f';
	char cons_e = 'A';
	char cons_f = 'F';


	if(base < 10){
		cons_b = (char) (cons_a + base);

		cons_c = cons_a;
		cons_d = cons_b;
		cons_e = cons_a;
		cons_f = cons_b;

	} else {
		cons_d = (char) (cons_c + (base - 10));
		cons_f = (char) (cons_e + (base - 10));
	}

	while(((str[a] < cons_a && str[a] > cons_b) || (str[a] < cons_c && str[a] > cons_d) || (str[a] < cons_e && str[a] > cons_f)) && str[a] != '\0'){
		a++;
	}

	if(a > 0){
		if(str[a-1] == '-'){
			neg = 1;
			*len += 1;
			str++;
		}
	}

	str += a;

	while(*str != '\0'){
		if(*str >= cons_e && *str <= cons_f){
			result += (*str - cons_e + 10) * base;
			*len += 1;
			str++;
		} else if(*str >= cons_c && *str <= cons_d){
			result += (*str - cons_c + 10) * base;
			*len += 1;
			str++;
		} else if(*str >= cons_a && *str <= cons_b){
			result += (*str - cons_a) * base;
			*len += 1;
			str++;
		} else {
			break;
		}
	}

	if(neg){
		result = result * -1;
	}

	return result;
}

/**
 * Converts a number to its equivalent with base 'base'
 * @param num: the integer to be converted
 * @param base: the base for the converted number
 * @return: a pointer to a char array with the new number
 */
char *convertToString(int num, int base) {
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

/**
 * Converts a double to a string
 * @param num: the double to be converted
 * @return: a pointer to a char array with the new number
 */
char *covertDoubleToString(double num){
	int i = 0;
	for(i = 0; i < LONG_MAX; i++){
		if(num - i < 0){
			break;
		}
	}

	char buffer[128];
	char *ptr = buffer;

	double decimals = num - (double) i + 1.0;
	int integers = i - 1;

	char *high = convertToString(integers, 10);
	int len = strlen(high);

	for(i = 0; i < len; i++){
		*ptr = high[i];
		ptr++;
	}

	*ptr = '.';

	for(i = 0; i < len; i++){
		//TODO: Finish this,
	}

}

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

		const char* format_begun_at = format++;

		int zero_padding = 0;
		int prefix_enable = 0;
		int min_length = 0;
		int min_decimals = 0;


		if(*format == '#'){
			prefix_enable = 1;
			format++;
		}

		if(*format == '0'){
			zero_padding = 1;
			format++;
		}

		if(*format > '0' && *format <= '9'){
			int numlen = 0;
			min_length = convertToNumber(format, &numlen, 10);

			format += numlen;
		}

		if(*format == '.'){
			format++;
			if(*format > '0' && *format <= '9'){
				int numlen = 0;
				min_decimals = convertToNumber(format, &numlen, 10);

				format += numlen;
			}
		}

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
		} else if (*format == 'X') {
			format++;
			int hex = va_arg(parameters, int);

			const char *str = convertToString(hex, 16);
			size_t len = strlen(str);

			if(hex != 0 && prefix_enable){
				if(max_remaining >= 2){
					if (!print("0X", 2)){
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

			const char *str = convertToString(dec, 10);
			size_t len = strlen(str);

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
		} else if (*format == 'B') {
			format++;
			int bin = va_arg(parameters, int);

			const char *str = convertToString(bin, 2);
			size_t len = strlen(str);

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
		} else {
			format = format_begun_at;
			size_t len = strlen(format);

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
