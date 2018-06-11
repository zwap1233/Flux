#system makefile

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

C_OBJS:=$(shell find kernel -name *.c)
CPP_OBJS:=$(shell find kernel -name *.cpp)
KERNEL_OBJS:=$(C_OBJS:.c=.o) $(CPP_OBJS:.cpp=.o) $(KERNEL_ARCH_OBJS)

PREFIX_ENTRIES?=\
$(ARCHDIR)/crtend.o \
$(ARCHDIR)/crtn.o \

SUFFIX_ENTRIES?=\
$(ARCHDIR)/crti.o \
$(ARCHDIR)/crtbegin.o \

OBJS=\
$(SUFFIX_ENTRIES) \
$(KERNEL_OBJS) \
$(PREFIX_ENTRIES) \

LINK_LIST=\
$(LDFLAGS) \
$(SUFFIX_ENTRIES) \
$(KERNEL_OBJS) \
$(LIBS) \
$(PREFIX_ENTRIES) \

.PHONY: all clean install install-headers install-binairies
.SUFFIXES: .o .c .cpp .S

all: $(OBJS) $(ARCHDIR)/$(LINKER)

$(KERNFILE): $(OBJS) $(ARCHDIR)/$(LINKER) FORCE
	$(CC) -T $(ARCHDIR)/$(LINKER) -o $@ $(CFLAGS) $(LINK_LIST)

$(ARCHDIR)/crtbegin.o $(ARCHDIR)/crtend.o:
	@OBJ=`$(CC) $(CFLAGS) $(LDFLAGS) -print-file-name=$(@F)` && cp "$$OBJ" $@

.c.o:
	$(CC) -MD -c $< -o $@ $(CFLAGS) -std=gnu11

.cpp.o:
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

.S.o:
	$(CC) -MD -c $< -o $@ $(CFLAGS)
	
FORCE:

clean:
	@rm -f $(KERNFILE)
	@rm -f $(OBJS) *.o */*.o */*/*.o
	@rm -f $(OBJS:.o=.d) *.d */*.d */*/*.d

install: install-headers install-binairies

install-headers:
	@echo 'Install sys headers'
	@mkdir -p $(DESTDIR)$(INCLUDEDIR)
	@cp -R --preserve=timestamps include/. $(DESTDIR)$(INCLUDEDIR)/.

install-binairies: $(KERNFILE)
	@echo 'Installed kernel'
	@mkdir -p $(DESTDIR)$(BOOTDIR)
	@cp $(KERNFILE) $(DESTDIR)$(BOOTDIR)

-include $(OBJS:.o=.d)
