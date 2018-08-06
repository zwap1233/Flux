/*
 * core_test.c
 *
 *  Created on: Jul 24, 2018
 *      Author: computation
 */

#include <UnitTesting/test_cases.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void run_cases(){
	printf("\n========= Running libk test =========\n\n");

	printf_test_case();
	atoi_test_case();

	memcmp_test_case();
	//memcpy_test_case();
	//memmove_test_case();
	//memset_test_case();
	//strlen_test_case();

	printf("\n========= end =========\n\n");
}

void printf_test_case(){
	printf("Just Text\n");
	printf("Some Text:%s\n\n", "Some Text");

	printf("5:%d\n", 5);
	printf("45:%d\n", 45);
	printf("-35:%d\n", -35);
	printf("0:%d\n\n", 0);

	printf("10:%b\n", 0b0010);
	printf("0b111:%#b\n", 0b111);
	printf("0011:%04b\n", 0b11);
	printf("0b00010:%#05b\n\n", 0b10);

	printf("FF22:%x\n", 0xFF22);
	printf("0xFF2:%#x\n", 0xFF2);
	printf("00A345:%06x\n", 0xA345);
	printf("0x00A345:%#06x\n\n", 0xA345);
}

void atoi_test_case(){
	printf("45:%d\n", (int) atoi("45"));
	printf("-456:%d\n", (int) atoi("-456"));
	printf("45:%d\n", (int) atoi("045"));
	printf("-456:%d\n", (int) atoi("-0456"));
	printf("-456:%d\n\n", (int) atoi("0-456"));
}

void memcmp_test_case(){
	char *a = "String 1";
	char *b = "String 1";
	char *c = "String 2";

	if(memcmp(a, b, 8) != 0){printf("Error memcmp test case 1 fail\n");}
	if(memcmp(a, c, 8) == 0){printf("Error memcmp test case 2 fail\n");}


}

void memcpy_test_case(){
	char *a = "String 1";
	char *b = "String 1";
	char *c = "String 2";

	memcpy(c, b, 9);
	if(memcmp(a, b, 9)){printf("Error memcpy test case 1 fail\n");}
}

void memmove_test_case(){
	char *a = "String 1";
	char *b = "String 1";
	char *c = "String 2";

	memmove(c, b, 9);
	if(memcmp(a, b, 9)){printf("Error memmove test case 1 fail\n");}
}

void memset_test_case(){
	char *a = "String 1";

	memset(a, 0, 9);
	if(memcmp(a, "String 1", 9)){printf("Error memset test case 1 fail\n");}
}

void strlen_test_case(){
	char *a = "String 1";
	char *b = "String 1asd";
	char *c = "Strin221g2";
	char *d = "S";

	if(strlen(a) != 9){printf("Error strlen test case 1 fail\n");}
	if(strlen(b) != 12){printf("Error strlen test case 2 fail\n");}
	if(strlen(c) != 11){printf("Error strlen test case 3 fail\n");}
	if(strlen(d) != 2){printf("Error strlen test case 4 fail\n");}
}


