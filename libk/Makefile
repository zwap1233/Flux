#Libk makefile

HOST?=i686-elf
HOSTARCH?=$(shell if echo $(HOST) | grep -Eq 'i[[:digit:]]86-'; then echo i386; else echo $(HOST) | grep -Eo '^[[:alnum:]_]*'; fi)

CFLAGS?=-O2 -g
CPPFLAGS?=-fno-exceptions -fno-rtti
LDFLAGS?=
LIBS?=

DESTDIR?=$(HOME)/sysroot
PREFIX?=/usr
SUFFIX?=/libk
LIBDIR?=$(PREFIX)/lib
INCLUDEDIR?=$(PREFIX)/include$(SUFFIX)

ARCHDIR=arch/$(HOSTARCH)

include $(ARCHDIR)/make.config

WARNINGS:=  -Wall -Wextra -pedantic -Wshadow -Wpointer-arith -Wcast-align \
            -Wwrite-strings -Wmissing-declarations \
            -Wredundant-decls -Winline -Wno-long-long \
            -Wuninitialized -Wconversion

CFLAGS:=$(CFLAGS) -ffreestanding -D__is_libc -Iinclude $(WARNINGS) $(ARCH_CFLAGS)
CPPFLAGS:=$(CPPFLAGS) $(ARCH_CPPFLAGS)
LIBK_CFLAGS:=$(CFLAGS) -D__is_libk $(KERNEL_ARCH_CFLAGS)
LIBK_CPPFLAGS:=$(CPPFLAGS) $(KERNEL_ARCH_CPPFLAGS)

FREESRC:=$(patsubst ./%, %, $(shell find -name *.c -not -path arch))
FREEOBJS:=$(ARCH_FREEOBJS) $(FREESRC:.c=.o) 

HOSTEDOBJS=\
$(ARCH_HOSTEDOBJS) \

OBJS=\
$(FREEOBJS) \
$(HOSTEDOBJS) \

LIBK_OBJS=$(FREEOBJS:.o=.libk.o)

LIBK_FILE:=$(DESTDIR)$(LIBDIR)/libk.a
LIBC_FILE:=$(DESTDIR)$(LIBDIR)/libc.a
#BINARIES=libc.a libk.a # Not ready for libc yet.
BINARIES=$(LIBK_FILE)

.PHONY: all clean install install-headers install-binairies

all: $(BINARIES)

$(LIBC_FILE): $(OBJS)
	@mkdir -p $(DESTDIR)$(LIBDIR)
	$(AR) rcs $@ $(OBJS)

$(LIBK_FILE): $(LIBK_OBJS)
	@mkdir -p $(DESTDIR)$(LIBDIR)
	$(AR) rcs $@ $(LIBK_OBJS)

%.o: %.c
	$(CC) -MD -c $< -o $@ $(CFLAGS) -std=gnu11

%.S: %.c
	$(CC) -MD -c $< -o $@ $(CFLAGS)

%.libk.o: %.c
	$(CC) -MD -c $< -o $@ $(LIBK_CFLAGS)

%.libk.o: %.S
	$(CC) -MD -c $< -o $@ $(LIBK_CFLAGS)

clean:	
	@rm -f $(BINARIES) *.a
	@rm -f $(OBJS) $(LIBK_OBJS)
	@rm -f $(OBJS:.o=.d) $(LIBK_OBJS:.o=.d)

install: install-headers install-binairies

install-headers:
	@echo 'Install lib headers'
	@mkdir -p $(DESTDIR)$(INCLUDEDIR)
	@cp -R --preserve=timestamps include/. $(DESTDIR)$(INCLUDEDIR)/.

install-binairies: $(BINARIES)
	@echo 'Installed libk'

-include $(OBJS:.o=.d)
-include $(LIBK_OBJS:.o=.d)
