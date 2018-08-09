/*
 * descriptor_tables.h
 *
 *  Created on: Aug 1, 2018
 *      Author: computation
 */

#ifndef KERNEL_INCLUDE_CORE_DESCRIPTOR_TABLES_H_
#define KERNEL_INCLUDE_CORE_DESCRIPTOR_TABLES_H_

typedef struct {
	uint8_t byte[4];
} GDT_t;

void setupGDT();

void encodeGDTEntery(GDT_t *entry){

#endif /* KERNEL_INCLUDE_CORE_DESCRIPTOR_TABLES_H_ */
