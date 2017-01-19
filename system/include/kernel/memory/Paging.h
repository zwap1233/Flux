/*
 * Paging.h
 *
 *  Created on: Jan 6, 2017
 *      Author: wouter
 */

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

namespace Mem_Paging {

	const uint32_t virt_adr_kernel = 0xC0100000;

	void initPaging();

	uint64_t getPhysicalAddress(uint32_t virt);

	uint64_t allocPage();

	uint64_t allocPage(int pages);

	void setPageEntry(uint64_t phys, uint32_t virt, bool RW, bool US, bool PWT, bool PCD, bool Global);

	void setPageAddr(uint64_t phys, uint32_t virt);

	void setPageFlags(uint32_t virt, bool RW, bool US, bool PWT, bool PCD, bool Global);
}


