#include <kernel.h>

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include <kernel/io/vga.h>
#include <kernel/memory/paging.h>

extern uint32_t ld_kernel_start;
extern uint32_t ld_kernel_end;

void earlyKernel(void);
void mainKernel(void);

/**
 * Early init point for kernel, is called by boot.S before the global constructors are initialized
 */
void earlyKernel(void){
	__stack_chk_guard_setup();

	//TODO: Call constructors of static and global objects
}

/**
 * Starting point for the kernel
 */
void mainKernel(void) {
	vga_init(&ld_kernel_start, &ld_kernel_end);
	//printf("Hello, kernel World!\n");
}

__attribute__((noreturn))
void panic(const char *msg){
	printf(msg);

	while(1){};
}


/************************************************************
 * 		Stack smashing protector
 */

uintptr_t __stack_chk_guard = 0x2a6d00f3;


__attribute__((noreturn))
void __stack_chk_fail();

void __stack_chk_guard_setup(void){
	//TODO: implement a more complex way to randomize the guard value
}

__attribute__((noreturn))
void __stack_chk_fail(void){
	panic("Stack smash detected");
}
