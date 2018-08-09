
#include <core/descriptor_tables.h>

volatile GDT_t gdt[1];

void setupGDT(){

}

void encodeGDTEntery(GDT_t *entry, uint32_t base, uint32_t limit, uint8_t present, uint8_t privilege, uint8_t excecute, uint8_t direction,){

}
