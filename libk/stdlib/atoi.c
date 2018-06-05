/*
 * atoi.c
 *
 *  Created on: Jun 5, 2018
 *      Author: computation
 */

#include <stdlib.h>

int atoi(const char *str){
	int result = 0;
	int neg = 0;

	int a;
	for(a = 0; (str[a] < '0' || str[a] > '9') && str[a] != '\0'; a++){;}

	if(str[a] == '\0'){
		return 0;
	}

	if(a > 0){
		if(str[a-1] == '-'){
			neg = 1;
		}
	}

	str += a;

	for(a = 0; (str[a] >= '0' && str[a] <= '9') && str[a] != '\0'; a++){;}

	for(; a >= 0; a--){
		result += (str[a] - '0') * (10^a);
	}

	if(neg){
		result = result * -1;
	}

	return result;
}

