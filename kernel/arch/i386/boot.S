# Declare constants for the multiboot header.
.set ALIGN,    1<<0             # align loaded modules on page boundaries
.set MEMINFO,  1<<1             # provide memory map
.set FLAGS,    ALIGN | MEMINFO  # this is the Multiboot 'flag' field
.set MAGIC,    0x1BADB002       # 'magic number' lets bootloader find the header
.set CHECKSUM, -(MAGIC + FLAGS) # checksum of above, to prove we are multiboot

.extern _paging_init

# Declare a header as in the Multiboot Standard.
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM


.section .bss
.global boot_info_p
boot_info_p: .long

# Reserve a stack for the initial thread.
.section .bss_stack, "aw", @nobits
.align 16
stack_bottom:
.skip 16384 # 16 KiB
stack_top:

# The kernel entry point.
.section .text
.global _start
.global _paging_return
.type _start, @function
_start:

	#store boot info
	addl 0xC0000000, %ebx
	movl %ebx, MEM_INFO_P - 0xC0000000

	#setup paging
	lea _paging_init - 0xC0000000, %ecx
	jmp *%ecx
_paging_return:

	movl $(stack_top), %esp

	# Early call to the kernel
	call earlyKernel

	# Call the global constructors.
	call _init

	# Transfer control to the main kernel.
	call mainKernel

	# Hang if kernel_main unexpectedly returns.
	cli
4:	hlt
	jmp 4b
.size _start, . - _start
