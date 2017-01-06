/*
 * Paging.cpp
 *
 *  Created on: Jan 6, 2017
 *      Author: wouter
 */

#include <kernel/memory/Paging.h>

using namespace Mem_Paging;

/**
 * Setup paging, should be called only once by boot code
 */
void Mem_Paging::initPaging(){

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
