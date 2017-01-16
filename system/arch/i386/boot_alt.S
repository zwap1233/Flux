# Declare constants for the multiboot header.
.set ALIGN,    1<<0             # align loaded modules on page boundaries
.set MEMINFO,  1<<1             # provide memory map
.set FLAGS,    ALIGN | MEMINFO  # this is the Multiboot 'flag' field
.set MAGIC,    0x1BADB002       # 'magic number' lets bootloader find the header
.set CHECKSUM, -(MAGIC + FLAGS) # checksum of above, to prove we are multiboot

.set VGA_BUF, 0x3FF000

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
.globl pagedir
.globl pagetabel
.globl pagetabel1
.globl pagepointer
pagedir:
	.skip 4096
pagetabel:
	.skip 4096
pagetabel1:
	.skip 4096
pagepointer:
	.skip 4096

# The kernel entry point.
.section .text
.global _start
.type _start, @function
_start:

	# set the page pointer table third index to the page dir
	# to map from 0xC0000000 the page dir must be mapped to the third entry in the page pointer table
	movl $(pagedir - 0xC0000000), (pagepointer - 0xC0000000 + 24)
	movl $0x0, (pagepointer - 0xC0000000 + 28)

	# temporarily identity map the first dir
	movl $(pagedir - 0xC0000000), (pagepointer - 0xC0000000)
	movl $0x0, (pagepointer - 0xC0000000 + 4)


	# Fill page dir
	# to map one MiB two page tables are needed, last entrys are mapped to the page dirs themself so are not used
	movl $(pagetabel - 0xC0000000), (pagedir - 0xC0000000)
	movl $(0x03), (pagedir - 0xC0000000 + 4)

	movl $(pagetabel1 - 0xC0000000), (pagedir - 0xC0000000 + 8)
	movl $(0x03), (pagedir - 0xC0000000 + 12)

	movl $(pagepointer - 0xC0000000), (pagedir - 0xC0000000 + 8*507)
	movl $(0x03), (pagedir - 0xC0000000 + 8*507 + 4)

	# second and third pagedirs still have to be mapped here
	movl $(pagedir - 0xC0000000), (pagedir - 0xC0000000 + 8*511)
	movl $(0x03), (pagedir - 0xC0000000 + 8*511 + 4)

	# start filling pagetable at address 0
	movl $1023, %ecx
	# map physical address 0 to virtual address 0xC0000000
	movl $0, %esi
	movl $0xC0000000, %edi
1:
	movl %esi, %edi
	addl $4, %edi

	# set readable and writable and present
	movl $(0x03), %edi

	addl $4096, %esi
	addl $4, %edi

	# this will run through both page tables at the same time, so they have to be following each other
	loop 1b

	# seperate mapping for vga buffer
	movl $(VGA_BUF), (pagetabel - 0xC0000000 + 1023 * 8)
	movl $(0x3), (pagetabel - 0xC0000000 + 1023 * 8 + 4)

	movl $(pagepointer - 0xC0000000), %ecx
	movl %ecx, %cr3

	movl %cr0, %ecx
	orl $0x80010000, %ecx
	movl %ecx, %cr0

	movl %cr4, %ecx
	orl $0x10, %ecx
	movl %ecx, %cr4

	leal 2f, %ecx
	jmp *%ecx
2:
	# unmap identity paging
	movl $0, pagepointer
	movl $0, pagepointer + 4

	movl $stack_top, %esp

	# Call the early/init kernel function
	call initKernel

	# Call the global constructors.
	call _init

	# Transfer control to the main kernel.
	call mainKernel

	# Hang if kernel_main unexpectedly returns.
	cli
5:	hlt
	jmp 5b
.size _start, . - _start
