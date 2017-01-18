/*
 * Paging.cpp
 *
 *  Created on: Jan 6, 2017
 *      Author: wouter
 */

#include <kernel/memory/Paging.h>

using namespace Mem_Paging;

/*volatile uint64_t pagedir[512] __attribute__((aligned(4096)));
volatile uint64_t pagetabel[512] __attribute__((aligned(4096)));
volatile uint64_t pagetabel1[512] __attribute__((aligned(4096)));
volatile uint64_t page_dir_ptr[4] __attribute__((aligned(32)));*/

/**
 * Setup paging, should be called only once by boot code
 * Enables PEA paging mode
 */
void Mem_Paging::initPaging(){

/*
	page_dir_ptr[0] = 0;
	page_dir_ptr[1] = 0;
	page_dir_ptr[2] = 0;
	page_dir_ptr[3] = (uint64_t) &pagedir;

	pagedir[0] = (uint64_t) &pagetabel | 3;
	pagedir[1] = (uint64_t) &pagetabel1 | 3;

	uint64_t addr = 0;
	for(int i = 0; i < 512; i++){
		pagetabel[i] = (addr & 0xfffffffffffff000) | 3;	//map addr and mark as present and writable
		addr += 4096;
	}

	for(int i = 0; i < 511; i++){
		pagetabel1[i] = (addr & 0xfffffffffffff000) | 3;	//map addr and mark as present and writable
		addr += 4096;
	}
	pagetabel1[511] = 0x000B8000 | 3;

	uint64_t *page_dir = (uint64_t*) (page_dir_ptr[3] & 0xfffffffffffff000); // get the page directory and remove flags

	page_dir[511] = (uint64_t) page_dir; // map pd to itself
	page_dir[510] = page_dir_ptr[2]; // map pd3 to it
	page_dir[509] = page_dir_ptr[1]; // map pd2 to it
	page_dir[508] = page_dir_ptr[0]; // map pd1 to it
	page_dir[507] = (uint64_t) &page_dir_ptr; // map the PDPT to the directory

	asm volatile ("movl %%cr4, %%eax; bts $5, %%eax; movl %0, %%cr3; movl %%eax, %%cr4" :: "a" (&page_dir_ptr)); // set bit5 in CR4 to enable PAE
	*/
}

/**
 * Get the physical address that belongs to the given virtual address by addr_t virt
 *
 * @param virt
 */
void *Mem_Paging::getPhysicalAddress(addr_t virt){
	//TODO: Write function
}
