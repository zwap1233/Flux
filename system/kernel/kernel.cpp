#include <kernel/shell.h>
#include <stdio.h>
#include <stdint.h>
#include <kernel/memory/Paging.h>

extern uint32_t ld_kernel_start;
extern uint32_t ld_kernel_end;

extern uint32_t *boot_pagedir;
extern uint32_t *boot_pagetab1;

extern "C" {

	void initKernel(void);
	void mainKernel(void);

	extern void loadPageDirectory(unsigned int*);
	extern void enablePaging();


	/**
	 * Early init point for kernel, is called by boot.S before the global constructors are initialized
	 */
	void initKernel(void){
	}

	/**
	 * Starting point for the kernel
	 */
	void mainKernel(void) {
		shell::initialize(&ld_kernel_start, &ld_kernel_end);
		printf("Hello, kernel World!\n");
		printf("0x%x\n", &boot_pagedir);
		printf("0x%x\n", &boot_pagetab1);
	}
}

/***********************************************************8
 * 		c++ support functions
 */

namespace __cxxabiv1 {
	/* guard variables */

	/* The ABI requires a 64-bit type.  */
	__extension__ typedef int __guard __attribute__((mode(__DI__)));

	extern "C" int __cxa_guard_acquire (__guard *);
	extern "C" void __cxa_guard_release (__guard *);
	extern "C" void __cxa_guard_abort (__guard *);

	extern "C" int __cxa_guard_acquire (__guard *g) {
		return !*(char *)(g);
	}

	extern "C" void __cxa_guard_release (__guard *g){
		*(char *)g = 1;
	}

	extern "C" void __cxa_guard_abort (__guard *g){
		*(char *)g = 0;
	}
}

/**
 * virtual backup definition
 * is used by the compiler as the definition for a virtual function that is nowhere implemented
 */
extern "C" void __cxa_pure_virtual(){
    printf("ERROR: virtual function is called\n");
}
