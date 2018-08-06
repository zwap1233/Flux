#kernel makefile

HOST?=i686-elf
HOSTARCH?=$(shell if echo $(HOST) | grep -Eq 'i[[:digit:]]86-'; then echo i386; else echo $(HOST) | grep -Eo '^[[:alnum:]_]*'; fi)

CFLAGS?=-O2 -g
CPPFLAGS?=-fno-exceptions -fno-rtti
LDFLAGS?=
LIBS?=

DESTDIR?=$(HOME)/sysroot
PREFIX?=/usr
BOOTDIR?=/boot
INCLUDEDIR?=$(PREFIX)/include

LIBK:=$(shell find $(DESTDIR) -name libk.a)

ARCHDIR=arch/$(HOSTARCH)

include $(ARCHDIR)/make.config

WARNINGS:=  -Wall -Wextra -pedantic -Wshadow -Wpointer-arith -Wcast-align \
            -Wwrite-strings -Wmissing-declarations \
            -Wredundant-decls -Winline -Wno-long-long \
            -Wuninitialized -Wconversion

CFLAGS:=$(CFLAGS) -ffreestanding -D__is_kernel -Iinclude $(WARNINGS) $(KERNEL_ARCH_CFLAGS)
CPPFLAGS:=$(CPPFLAGS) $(KERNEL_ARCH_CPPFLAGS)
LDFLAGS:=$(LDFLAGS) $(KERNEL_ARCH_LDFLAGS)
LIBS:=$(LIBS) -nostdlib -lk -lgcc $(KERNEL_ARCH_LIBS)

KERNFILE?=Flux.bin
#Backup for linker, should be set in arch .config file
LINKER?=linker.ld

C_OBJS:=$(shell find . -path ./include -prune -o -path ./arch -prune -o -name *.c -print)
CPP_OBJS:=$(shell find . -path ./include -prune -o -path ./arch -prune -o -name *.cpp -print)
KERNEL_OBJS:=$(C_OBJS:.c=.o) $(CPP_OBJS:.cpp=.o) $(KERNEL_ARCH_OBJS)

SUFFIX_ENTRIES?=\
$(ARCHDIR)/crtend.o \
$(ARCHDIR)/crtn.o \

PREFIX_ENTRIES?=\
$(ARCHDIR)/crti.o \
$(ARCHDIR)/crtbegin.o \

OBJS=\
$(PREFIX_ENTRIES) \
$(KERNEL_OBJS) \
$(SUFFIX_ENTRIES) \

LINK_LIST=\
$(LDFLAGS) \
$(PREFIX_ENTRIES) \
$(KERNEL_OBJS) \
$(LIBS) \
$(SUFFIX_ENTRIES) \

.PHONY: all clean install install-headers install-binairies

all: $(OBJS) $(ARCHDIR)/$(LINKER)

$(KERNFILE): $(OBJS) $(ARCHDIR)/$(LINKER) $(LIBK)
	@mkdir -p $(DESTDIR)$(BOOTDIR)
	$(CC) -T $(ARCHDIR)/$(LINKER) -o $@ $(CFLAGS) $(LINK_LIST)

$(ARCHDIR)/crtbegin.o $(ARCHDIR)/crtend.o:
	@OBJ=`$(CC) $(CFLAGS) $(LDFLAGS) -print-file-name=$(@F)` && cp "$$OBJ" $@

%.o: %.c
	$(CC) -MD -c $< -o $@ $(CFLAGS) -std=gnu11

%.o: %.cpp
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

%.o: %.S
	$(CC) -MD -c $< -o $@ $(CFLAGS)
	
FORCE:

clean:
	@rm -f $(OBJS)
	@rm -f $(OBJS:.o=.d)

install: install-headers install-binairies

install-headers:
	@echo 'Install sys headers'
	@mkdir -p $(DESTDIR)$(INCLUDEDIR)
	@cp -R --preserve=timestamps include/. $(DESTDIR)$(INCLUDEDIR)/.

install-binairies: $(KERNFILE)
	@echo 'Installed kernel'

-include $(OBJS:.o=.d)
