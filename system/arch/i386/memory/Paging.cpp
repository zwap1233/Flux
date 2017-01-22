/*
 * Paging.cpp
 *
 *  Created on: Jan 6, 2017
 *      Author: wouter
 */

#include <kernel/memory/Paging.h>

using namespace memory;

//Entire pageing structure can be read from this point
extern uint64_t boot_pagepointer[4];

/**
 * Paging init function
 */
void memory::initializePaging(){

}

/**
 * Get the physical address that belongs to the given virtual address by addr_t virt
 * If virt isnt mapped this returns zero
 *
 * @param virt
 * @return		physical address, shouldnt be read or written to
 */
uint64_t memory::getPhysicalAddress(uint32_t virt){
	uint32_t ptrindex = virt >> 30;
	uint32_t dirindex = 0x3FE00000 & (virt >> 20);
	uint32_t tabindex = 0x1FF000 & (virt >> 12);
	if(ptrindex > 3 || dirindex > 511 || tabindex > 511){
		printf("ERROR getPhysicalAddress: invalid address");
	}

	//TODO: breaks pagging when called, why?

	//TODO: Uses a trick to get virtual address of page tabels and dirs, this works for now but should be made robust
	if((boot_pagepointer[ptrindex] & 0x1) != 0){
		uint64_t *pagedir = (uint64_t *) ((boot_pagepointer[ptrindex] | 0xC0000000) & 0xFFFFF000);
		if((pagedir[dirindex] & 0x1) != 0){
			uint64_t *pagetab = (uint64_t *) ((pagedir[dirindex] | 0xC0000000 ) & 0xFFFFF000);
			if((pagetab[tabindex] & 0x1) != 0){
				return (pagetab[tabindex] & 0xFFFFF000);
			}
		}
	}

	return 0;
}

/**
 * Allocate a single page
 * should only be called by memory mangement
 *
 * @return	physical address of allocated page
 */
uint64_t memory::allocPage(){
	return allocPage(1);
}

/**
 * Allocate several pages.
 * the allocated pages dont have to be in order
 *
 * @param pages		Amount of pages to be allocated
 * @return			physical address of allocated page
 */
uint64_t memory::allocPage(int pages){
	return 0;
}

/**
 * Map the physical address denoted by phys to the virtual address denoted by virt. the flags are then set
 *
 * @param phys
 * @param virt
 * @param RW		if true page is read only
 * @param US		if true user mode is allowed access to page
 * @param PWT		if true write-through mode is enabled
 * @param PCD		if true cache is not used
 * @param Global	if true page is set as global page
 */
void memory::setPageEntry(uint64_t phys, uint32_t virt, bool RW, bool US, bool PWT, bool PCD, bool Global){
	//TODO: Write body
}

/**
 * Map the virtual address denoted by virt to the physical address denoted by phys
 * only works if entry already exsist
 *
 * @param phys
 * @param virt
 */
void memory::setPageAddr(uint64_t phys, uint32_t virt){
	//TODO: Write body
}

/**
 * Set the flags for the page denoted by virt
 *
 * @param virt
 * @param RW		if true page is read only
 * @param US		if true user mode is allowed access to page
 * @param PWT		if true write-through mode is enabled
 * @param PCD		if true cache is not used
 * @param Global	if true page is set as global page
 */
void memory::setPageFlags(uint32_t virt, bool RW, bool US, bool PWT, bool PCD, bool Global){
	//TODO: Write body
}
