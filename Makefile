export PROJECTS:=libk kernel

install-arm: HOST=arm-none-eabi
install-i686: HOST=i686-elf

HOST?=i686-elf

export HOST

export HOSTARCH=$(shell if echo $(HOST) | grep -Eq 'i[[:digit:]]86-'; then echo i386; else echo $(HOST) | grep -Eo '^[^-]*'; fi)

export AR=$(HOST)-ar
export AS=$(HOST)-as
export CC=$(HOST)-gcc

export SYSROOT:=$(shell pwd)/sysroot
export DESTDIR=$(SYSROOT)

export PREFIX:=
export BOOTDIR:=/boot
export LIBDIR:=/lib
export INCLUDEDIR:=$(PREFIX)/include

GRUBDIR:=/grub
GRUBCFG:=grub.cfg

export CFLAGS:=-O0 -g -fstack-protector-all
export CPPFLAGS:=-fno-exceptions -fno-rtti

# Configure the cross-compiler to use the desired system root.
export CFLAGS:=--sysroot=$(SYSROOT) $(CFLAGS)

# Work around that the -elf gcc targets doesn't have a system include directory
# because it was configured with --without-headers rather than --with-sysroot.
export CFLAGS:=-isystem=$(INCLUDEDIR) $(CFLAGS)

#inbuild vars
OSNAME=Flux
export KERNFILE:=$(DESTDIR)$(BOOTDIR)/$(OSNAME).bin
ISOFILE:=$(OSNAME).iso

.PHONEY: all install clean install-arm install-i686

all: sysroot
	@for p in $(PROJECTS); do cd ./$$p; $(MAKE) install-headers; cd ..; done
	@for p in $(PROJECTS); do cd ./$$p; $(MAKE) install-binairies; cd ..; done
	
	@grub-file --is-x86-multiboot2 $(KERNFILE); \
	if [ "$$?" -eq "1" ] ; then \
		echo 'ERROR: Kernel file is not multiboot compatible'; \
	fi;
	
install: all
	@mkdir -p $(DESTDIR)$(BOOTDIR)$(GRUBDIR)
	@cp $(GRUBCFG) $(DESTDIR)$(BOOTDIR)$(GRUBDIR)
	@grub-mkrescue -o $(ISOFILE) $(DESTDIR)
	
sysroot:
	@mkdir $(SYSROOT)

install-arm: HOST=arm-none-eabi
install-arm: install

install-i686: HOST=i686-elf
install-i686: install

clean:
	@for p in $(PROJECTS); do cd ./$$p; $(MAKE) clean; cd ..; done
	
	@rm -rf $(SYSROOT) && echo 'Removed: $(SYSROOT)'
	@rm -rf $(ISOFILE) && echo 'Removed: $(ISOFILE)'
