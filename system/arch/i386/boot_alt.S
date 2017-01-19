# Declare constants for the multiboot header.
.set ALIGN,    1<<0             # align loaded modules on page boundaries
.set MEMINFO,  1<<1             # provide memory map
.set FLAGS,    ALIGN | MEMINFO  # this is the Multiboot 'flag' field
.set MAGIC,    0x1BADB002       # 'magic number' lets bootloader find the header
.set CHECKSUM, -(MAGIC + FLAGS) # checksum of above, to prove we are multiboot

# Declare a header as in the Multiboot Standard.
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

# Reserve a stack for the initial thread.
.section .bss_stack, "aw", @nobits
.align 16
stack_bottom:
.skip 16384 # 16 KiB
stack_top:

# Reserve space for the page directory and first page table
.section .bss, "aw", @nobits
.align 4096
.globl boot_pagedir
.globl boot_pagetabel
.globl boot_pagetabel1
.globl boot_pagepointer
boot_pagedir:
	.skip 4096, 0
boot_pagetabel:
	.skip 4096, 0
boot_pagetabel1:
	.skip 4096, 0
boot_pagepointer:
	.skip 4096, 0

# The kernel entry point.
.section .text
.global _start
.type _start, @function
_start:

	# Physical address of boot_pagetab1.
	# TODO: I recall seeing some assembly that used a macro to do the
	#       conversions to and from physical. Maybe this should be done in this
	#       code as well?
	movl $(boot_pagetabel - 0xC0000000), %edi
	# First address to map is address 0.
	# TODO: Start at the first kernel page instead. Alternatively map the first
	#       1 MiB as it can be generally useful, and there's no need to
	#       specially map the VGA buffer.
	movl $0, %esi
	# Map 1024 pages. Each page table contains 512 entries so we run through bot tables in one run.
	# It is important that the page tables directly follow eachother
	# It maps the entire first 1 MiB so there is no need the specificly map the VGA BUF
	movl $1024, %ecx
1:
	# Set higher half of table entry to zero. all mapped locations will be below 1 GIB and thus not needed
	movl $0x0, (%edi)

	# add 4 bytes to point %edi to the lower half of the table entry
	addl $4, %edi

	# Map physical address as "present, writable". Note that this maps
	# .text and .rodata as writable. Mind security and map them as non-writable.
	movl %esi, %edx
	orl $0x003, %edx
	movl %edx, (%edi)

	# Size of page is 4096 bytes.
	addl $4096, %esi
	# Size of entries in boot_pagetab1 is 8 bytes but we are first moving to the higher half of the next entry.
	addl $4, %edi
	# Loop to the next entry if we haven't finished.
	loop 1b

2:
	# !NO NEED FOR THIS ANYMORE!
	# Map VGA video memory to 0xC03FF000 as "present, writable".
	#movl $(0x000B8000 | 0x003), boot_pagetabel - 0xC0000000 + 1023 * 4

	# Mapping page tables to first and second entry of the pagedir
	# When placing the pagedir in the first entry of the page dir pointer table the first 1 MiB
	# is automaticly identity mapped. When placing the pagedir in the last entry of the pointer table
	# the first 1 MiB is mapped to 0xC0000000.
	movl $(boot_pagetabel - 0xC0000000), %edx
	orl $0x03, %edx
	movl %edx, boot_pagedir - 0xC0000000 + 4
	movl $0x0, boot_pagedir - 0xC0000000

	movl $(boot_pagetabel1 - 0xC0000000), %edx
	orl $0x03, %edx
	movl %edx, boot_pagedir - 0xC0000000 + 12
	movl $0x0, boot_pagedir - 0xC0000000 + 8

	# mapping the page dir pointer table
	movl $(boot_pagepointer - 0xC0000000), %edx
	orl $0x03, %edx
	movl %edx, boot_pagedir - 0xC0000000 + 8*507 + 4
	movl $0x0, boot_pagedir - 0xC0000000 + 8*507

	# second and third pagedirs still have to be mapped here

	movl $(boot_pagedir - 0xC0000000), %edx
	orl $0x03, %edx
	movl %edx, boot_pagedir - 0xC0000000 + 8*511 + 4
	movl $0x0, boot_pagedir - 0xC0000000 + 8*511

	# set page pointer table entrys
	movl $(boot_pagedir - 0xC0000000), %edx
	orl $0x03, %edx
	movl %edx, boot_pagepointer - 0xC0000000 + 4
	movl %edx, boot_pagepointer - 0xC0000000 + 28
	movl $0x0, boot_pagepointer - 0xC0000000
	movl $0x0, boot_pagepointer - 0xC0000000 + 24

	# Set cr3 to the address of the boot_page_directory.
	movl $(boot_pagepointer - 0xC0000000), %ecx
	movl %ecx, %cr3

	# Enable paging and the write-protect bit.
	movl %cr0, %ecx
	orl $0x80010000, %ecx
	movl %ecx, %cr0

	# Jump to higher half with an absolute jump.
	lea 3f, %ecx
	jmp *%ecx

3:
	# At this point, paging is fully set up and enabled.

	# Unmap the identity mapping as it is now unnecessary.
	movl $0, boot_pagepointer + 4

	# Reload crc3 to force a TLB flush so the changes to take effect.
	movl %cr3, %ecx
	movl %ecx, %cr3

	movl $stack_top, %esp

	# Call the early/init kernel function
	call initKernel

	# Call the global constructors.
	call _init

	# Transfer control to the main kernel.
	call mainKernel

	# Hang if kernel_main unexpectedly returns.
	cli
4:	hlt
	jmp 4b
.size _start, . - _start
