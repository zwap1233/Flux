/*
 * Paging.cpp
 *
 *  Created on: Jan 6, 2017
 *      Author: wouter
 */

#include <kernel/memory/Paging.h>

using namespace Mem_Paging;

/*volatile uint64_t page_dir_ptr_tab[4] __attribute__((aligned(32))); // must be aligned to (at least)0x20 (32 dec), ...

volatile uint64_t page_dir[512][4] __attribute__((aligned(4096)));  // must be aligned to page boundary
volatile uint64_t page_tab[512] __attribute__((aligned(4096)));*/

/**
 * Setup paging, should be called only once by boot code
 */
void Mem_Paging::initPaging(){
	/*page_dir_ptr_tab[0] = ((uint64_t) &page_dir[0]) | 1; 	// set the page directory into the PDPT and mark it present
	page_dir_ptr_tab[1] = ((uint64_t) &page_dir[1]) | 1;
	page_dir_ptr_tab[2] = ((uint64_t) &page_dir[2]) | 1;
	page_dir_ptr_tab[3] = ((uint64_t) &page_dir[3]) | 1;

	page_dir[0][0] = ((uint64_t) &page_tab ) | 3; 			//set the page table into the PD and mark it present/writable

	uint64_t address = 0;
	for(int i = 0; i < 512; i++){
		page_tab[i] = address | 3;						//Set the address in the page table and set the read/write bit and present bit
		address += 0x1000;								//Add 4 Kib to the address aka next page
	}

	asm volatile ("movl %cr4, %eax; bts $5, %eax; movl %eax, %cr4"); 	// set bit5 in CR4 to enable PAE
	asm volatile ("movl %%eax, %%cr3" :: "a" (&page_dir_ptr_tab)); 		// load PDPT into CR3

	asm volatile ("movl %cr0, %eax; orl $0x80000000, %eax; movl %eax, %cr0;");	//Enable paging by setting bit 31 in CR0

	uint64_t * page_dir_ptr = (uint64_t*) page_dir_ptr_tab[3]; 	// get the page directory (you should 'and' the flags away)
	page_dir_ptr[511] = (uint64_t)page_dir_ptr;						// map pd to itself
	page_dir_ptr[510] = page_dir_ptr_tab[2];					// map pd3 to it
	page_dir_ptr[509] = page_dir_ptr_tab[1];					// map pd2 to it
	page_dir_ptr[508] = page_dir_ptr_tab[0];					// map pd1 to it
	page_dir_ptr[507] = (uint64_t) &page_dir_ptr_tab;			// map the PDPT to the directory*/
}

/**
 * Convert the physical address phys to a virtual address
 *
 * @param phys
 * @return
 */
uint32_t Mem_Paging::conv_phys_to_virt(uint32_t phys){
	//TODO: Write function
	return -1;
}

/**
 * Convert the virtual address virt to a physical address
 *
 * @param virt
 * @return
 */
uint32_t Mem_Paging::conv_virt_to_phys(uint32_t virt){
	//TODO: Write function
	return -1;
}
