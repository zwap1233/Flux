.extern _paging_return

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
	.skip 32, 0

.section .text
.global _paging_init
.type _paging_init, @function
_paging_init:

	# Physical address of boot_pagetabel.
	movl $(boot_pagetabel - 0xC0000000), %edi

	# First address to map is address 0.
	movl $0, %esi

	# Map 512 pages. Each page table contains 512 entries so we have to run through the tables sepretly.
	# It maps the entire first 4 MiB so there is no need the specificly map the VGA BUF
	movl $1024, %ecx
1:
	# Map physical address as "present, writable". Note that this maps
	# .text and .rodata as writable. Mind security and map them as non-writable.
	movl %esi, %edx
	andl $0xFFFFF000, %edx
	orl $0x003, %edx
	movl %edx, (%edi)

	# add 4 bytes to point %edi to the higher half of the table entry
	addl $4, %edi

	# Set higher half of table entry to zero. all mapped locations will be below 1 GiB and
	# thus this part is not needed. Because the file is in little endian, meaning the least
	# significant byte comes first in memory, the address must be loaded first and then the rest
	movl $0x0, (%edi)

	# Size of page is 4096 bytes.
	addl $4096, %esi
	# Size of entries in boot_pagetabel is 8 bytes but we are first moving to the higher half of the next entry.
	addl $4, %edi
	# Loop to the next entry if we haven't finished.
	loop 1b
2:
	# !NO NEED FOR THIS ANYMORE!
	# Map VGA video memory to 0xC03FF000 as "present, writable".
	#movl $(0x000B8000 | 0x003), boot_pagetabel - 0xC0000000 + 1023 * 4

	# Mapping page tables to first and second entry of the pagedir
	# When placing the pagedir in the first entry of the page dir pointer table the first 4 MiB
	# is automaticly identity mapped. When placing the pagedir in the last entry of the pointer table
	# the first 4 MiB is mapped to 0xC0000000.
	movl $(boot_pagetabel - 0xC0000000), %edx
	andl $0xFFFFF000, %edx
	orl $0x03, %edx
	movl %edx, boot_pagedir - 0xC0000000
	movl $0x0, boot_pagedir - 0xC0000000 + 4

	movl $(boot_pagetabel1 - 0xC0000000), %edx
	andl $0xFFFFF000, %edx
	orl $0x03, %edx
	movl %edx, boot_pagedir - 0xC0000000 + 8
	movl $0x0, boot_pagedir - 0xC0000000 + 12

	# mapping the page dir pointer table
	movl $(boot_pagepointer - 0xC0000000), %edx
	andl $0xFFFFF000, %edx
	orl $0x03, %edx
	movl %edx, boot_pagedir - 0xC0000000 + 8*507
	movl $0x0, boot_pagedir - 0xC0000000 + 8*507 + 4

	#TODO: second and third pagedirs still have to be mapped here

	movl $(boot_pagedir - 0xC0000000), %edx
	andl $0xFFFFF000, %edx
	orl $0x03, %edx
	movl %edx, boot_pagedir - 0xC0000000 + 8*511
	movl $0x0, boot_pagedir - 0xC0000000 + 8*511 + 4

	# set page pointer table entrys
	movl $(boot_pagedir - 0xC0000000), %edx
	andl $0xFFFFF000, %edx
	orl $1, %edx
	#identity mapping, so TLB can find the pages for the first time
	movl %edx, boot_pagepointer - 0xC0000000
	movl $0x0, boot_pagepointer - 0xC0000000 + 4
	#final location of the page table enteries
	movl %edx, boot_pagepointer - 0xC0000000 + 24
	movl $0x0, boot_pagepointer - 0xC0000000 + 28

	# Set cr3 to the address of the boot_page_directory.
	movl $(boot_pagepointer - 0xC0000000), %ecx
	movl %ecx, %cr3

	# enable PEA paging mode
	movl %cr4, %edx
	orl $0x20, %edx
	movl %edx, %cr4

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
	movl $0, boot_pagepointer
	movl $0, boot_pagepointer + 4

	# Reload crc3 to force a TLB flush so the changes to take effect.
	movl %cr3, %ecx
	movl %ecx, %cr3

	movl %cr3, %ecx
	movl %ecx, %cr3

	#load the return address of the main function and jump to there
	lea _paging_return, %ecx
	jmp *%ecx
