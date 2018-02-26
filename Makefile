PROJECTS:=libk system

export HOST=i686-elf
export HOSTARCH:=$(shell if echo $(HOST) | grep -Eq 'i[[:digit:]]86-'; then echo i386; else echo $(HOST) | grep -Eo '^[[:alnum:]_]*'; fi)

export AR:=$(HOST)-ar
export AS:=$(HOST)-as
export CC:=$(HOST)-g++

export PREFIX=/usr
export BOOTDIR=/boot
export LIBDIR:=$(PREFIX)/lib
export INCLUDEDIR:=$(PREFIX)/include

export CFLAGS:=-O2 -g -fstack-protector-all
export CPPFLAGS:=-fno-exceptions -fno-rtti

#inbuild vars
OSNAME=Flux
export KERNFILE:=$(OSNAME).bin
export ISOFILE:=$(OSNAME).iso
ISODIR=isodir

export SYSROOT:=$(shell pwd)/sysroot
export DESTDIR=$(SYSROOT)

# Configure the cross-compiler to use the desired system root.
export CC:=$(CC) --sysroot=$(SYSROOT)

# Work around that the -elf gcc targets doesn't have a system include directory
# because it was configured with --without-headers rather than --with-sysroot.
export CC:=$(CC) -isystem=$(INCLUDEDIR)

.PHONEY: all install clean sysroot $(PROJECTS) FORCE

all: TARGET=install
all: sysroot $(PROJECTS)
	@grub-file --is-x86-multiboot system/$(KERNFILE) \\
	@if [ "$?" -eq "1" ] ; then \\
	@	echo 'ERROR: Kernel file is not multiboot' \\
	@fi

$(PROJECTS): FORCE
	@cd $@ && $(MAKE) $(TARGET)

FORCE:
	
sysroot:
	@mkdir $(SYSROOT)

clean: TARGET=clean
clean: $(PROJECTS)
	@rm -rf $(SYSROOT) && echo 'Removed: $(SYSROOT)'
	@rm -rf $(ISODIR) && echo 'Removed: $(ISODIR)'
	@rm -rf $(ISOFILE) && echo 'Removed: $(ISOFILE)'

install: all
	@mkdir -p $(ISODIR)
	@mkdir -p $(ISODIR)/boot
	@mkdir -p $(ISODIR)/boot/grub
	
	@cp $(SYSROOT)/boot/$(KERNFILE) $(ISODIR)/boot/$(KERNFILE)
	@cp grub.cfg $(ISODIR)/boot/grub/grub.cfg
	@grub-mkrescue -o $(ISOFILE) $(ISODIR)