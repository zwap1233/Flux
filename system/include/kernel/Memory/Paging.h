/*
 * Paging.h
 *
 *  Created on: Jan 6, 2017
 *      Author: wouter
 */

#include <stdio.h>
#include <stddef.h>

namespace Mem_Paging {

	const uint32_t virt_adr_kernel = 0xC0100000;

	void initPaging();

	uint32_t conv_phys_to_virt(uint32_t phys);
	uint32_t conv_virt_to_phys(uint32_t virt);
}


