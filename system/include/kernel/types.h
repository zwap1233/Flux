/*
 * kernel_types.h
 *
 *  Created on: Jan 16, 2017
 *      Author: wouter
 */

#ifndef SYSTEM_INCLUDE_TYPES_H_
#define SYSTEM_INCLUDE_TYPES_H_

#ifdef _IS_X86_64_
	typedef uint64_t addr_t;
#else
	typedef uint32_t addr_t;
#endif



#endif /* SYSTEM_INCLUDE_TYPES_H_ */
