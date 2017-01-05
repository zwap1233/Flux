#include <kernel/shell.h>
#include <stdio.h>

extern uint32_t ld_kernel_start;
extern uint32_t ld_kernel_end;

volatile uint32_t pagedir[1024] __attribute__((aligned(4096)));
volatile uint32_t pagetable[1024] __attribute__((aligned(4096)));

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

		for(int i = 0; i < 1024; i++){
			pagedir[i] = 0x00000002;
		}

		//we will fill all 1024 entries in the table, mapping 4 megabytes
		for(int i = 0; i < 1024; i++){
		    // As the address is page aligned, it will always leave 12 bits zeroed.
		    // Those bits are used by the attributes ;)
			pagetable[i] = (i * 0x1000) | 3; // attributes: supervisor level, read/write, present.
		}

		pagedir[0] = (unsigned int) pagetable | 3;

		loadPageDirectory((unsigned int *) pagedir);
		enablePaging();

		shell::initialize(&ld_kernel_start, &ld_kernel_end);
		printf("Hello, kernel World!\n");
		printf("%x\n", &pagedir);
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
