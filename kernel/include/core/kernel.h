/*
 * system.h
 *
 *  Created on: Jan 22, 2017
 *      Author: wouter
 */

#ifndef SYSTEM_INCLUDE_KERNEL_H_
#define SYSTEM_INCLUDE_KERNEL_H_

__attribute__((noreturn))
void panic(const char *msg);

extern void __stack_chk_guard_setup();

#endif /* SYSTEM_INCLUDE_KERNEL_H_ */
