export PROJECTS:=libk system

export HOST?=i686-elf
export HOSTARCH=$(shell if echo $(HOST) | grep -Eq 'i[[:digit:]]86-'; then echo i386; else echo $(HOST) | grep -Eo '^[^-]*'; fi)

export AR=$(HOST)-ar
export AS=$(HOST)-as
export CC=$(HOST)-g++

export PREFIX=/usr
export BOOTDIR=/boot
export LIBDIR:=$(PREFIX)/lib
export INCLUDEDIR:=$(PREFIX)/include

export CFLAGS:=-O0 -g -fstack-protector-all
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

.PHONEY: all install clean install-arm install-i686 install-iso

all: install
	
clean:
	@for p in $(PROJECTS); do cd ./$$p; $(MAKE) clean; cd ..; done
	
	@rm -rf $(SYSROOT) && echo 'Removed: $(SYSROOT)'
	@rm -rf $(ISODIR) && echo 'Removed: $(ISODIR)'
	@rm -rf $(ISOFILE) && echo 'Removed: $(ISOFILE)'

install: sysroot
	@for p in $(PROJECTS); do cd ./$$p; $(MAKE) install-headers; cd ..; done
	@for p in $(PROJECTS); do cd ./$$p; $(MAKE) install; cd ..; done
	
	@grub-file --is-x86-multiboot system/$(KERNFILE); \
	if [ "$$?" -eq "1" ] ; then \
		echo 'ERROR: Kernel file is not multiboot'; \
	fi;

install-arm: HOST=arm-none-eabi
install-arm: install

install-i686: HOST=i686-elf
install-i686: install

install-iso: install
	@mkdir -p $(ISODIR)
	@mkdir -p $(ISODIR)/boot
	@mkdir -p $(ISODIR)/boot/grub
	
	@cp $(SYSROOT)/boot/$(KERNFILE) $(ISODIR)/boot/$(KERNFILE)
	@cp grub.cfg $(ISODIR)/boot/grub/grub.cfg
	@grub-mkrescue -o $(ISOFILE) $(ISODIR)

sysroot:
	@mkdir $(SYSROOT)
