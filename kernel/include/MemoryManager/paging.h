/*
 * Paging.h
 *
 *  Created on: Jan 6, 2017
 *      Author: wouter
 */

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>


void paging_init();

uint64_t paging_getPhysicalAddress(uint32_t virt);

uint64_t paging_allocPage();

uint64_t paging_allocPages(int pages);

void paging_setPageEntry(uint64_t phys, uint32_t virt, int RW, int US, int PWT, int PCD, int Global);

void paging_setPageAddr(uint64_t phys, uint32_t virt);

void paging_setPageFlags(uint32_t virt, int RW, int US, int PWT, int PCD, int Global);


