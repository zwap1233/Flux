#include <kernel.h>

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include <kernel/shell.h>
#include <kernel/memory/Paging.h>

extern uint32_t ld_kernel_start;
extern uint32_t ld_kernel_end;

extern "C" {

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
		shell::initialize(&ld_kernel_start, &ld_kernel_end);
		printf("Hello, kernel World!\n");

		printf("Test dec:%d\n", -1234);
		printf("Test bin:%b\n", -12455);
		printf("Test hex:%x\n", -13);
		printf("Test float:%f\n", -1223.023242);
	}
}

__attribute__((noreturn))
void panic(const char *msg){
	printf(msg);

	while(true){};
}


/************************************************************
 * 		Stack smashing protector
 */

uintptr_t __stack_chk_guard = 0x2a6d00f3;

extern "C" {

	__attribute__((noreturn))
	void __stack_chk_fail();

	void __stack_chk_guard_setup(void){
		//TODO: implement a more complex way to randomize the guard value
	}

	__attribute__((noreturn))
	void __stack_chk_fail(void){
		panic("Stack smash detected");
	}
}

/************************************************************
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
    printf("ERROR: virtual function is called but not implemented\n");
}
