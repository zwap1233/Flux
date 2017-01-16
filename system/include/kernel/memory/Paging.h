/*
 * Paging.h
 *
 *  Created on: Jan 6, 2017
 *      Author: wouter
 */

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <kernel/types.h>

namespace Mem_Paging {

	const addr_t virt_adr_kernel = 0xC0100000;

	void initPaging();

	void *getPhysicalAddress(addr_t virt);
}


