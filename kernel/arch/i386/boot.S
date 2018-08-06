# Declare constants for the multiboot header.
.set MAGIC,		0xE85250D6					# 'magic number' lets bootloader find the header
.set ARCH,		0							# The device architecture 0 for i386 protected mode
.set HEADLEN,	16							# this is the Multiboot 'flag' field
.set CHECKSUM,	-(MAGIC + ARCH + HEADLEN)	# checksum of above, to prove we are multiboot

.set TAG_END_TYPE,	0						#End Tag of size 8 and type 0, flag is 0 so optional
.set TAG_END_FLAGS,	0
.set TAG_END_SIZE,	8

.extern _paging_init

# Declare a header as in the Multiboot Standard.
.section .multiboot
.align 8
.long MAGIC
.long ARCH
.long HEADLEN
.long CHECKSUM

.word TAG_END_TYPE
.word TAG_END_FLAGS
.long TAG_END_SIZE

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
	movl %ebx, boot_info_p - 0xC0000000

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
