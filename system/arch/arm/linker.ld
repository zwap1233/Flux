/* The bootloader will look at this image and start execution at the symbol
   designated at the entry point. */
ENTRY(_start)

SECTIONS {
    /* QEMU loads the kernel in Flash here. I strongly suggest you look at the
     * memory map.
     */
    . = 0x80010000;
	
	ld_kernel_start = . ;
    /* Make sure our entry stub goes first */
    .stub : 
    { 
    	boot.o(.text)
    }
    
    .text :
    {
    	*(.text)
   	}
   	
    .rodata :
    {
    	*(.rodata)
    }
    
    .data :
    {
    	*(.data)
    }
    
    .bss :
    {
    	*(.bss COMMON)
    }

    STACK_BASE = .;
    . += 0x10000;
    STACK_TOP = .;
    
    ld_kernel_end = . ;
}