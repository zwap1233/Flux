#include <kernel/shell.h>
#include <stdio.h>
#include <stdint.h>
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
	}

	/**
	 * Starting point for the kernel
	 */
	void mainKernel(void) {
		shell::initialize(&ld_kernel_start, &ld_kernel_end);
		printf("Hello, kernel World!\n");
		//printf("%x", Mem_Paging::getPhysicalAddress(0xC00B8000));
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
