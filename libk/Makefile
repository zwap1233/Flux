#Libk makefile

HOST?=i686-elf
HOSTARCH:=$(shell if echo $(HOST) | grep -Eq 'i[[:digit:]]86-'; then echo i386; else echo $(HOST) | grep -Eo '^[[:alnum:]_]*'; fi)

CFLAGS?=-O2 -g
CPPFLAGS?=
LDFLAGS?=
LIBS?=

DESTDIR?=$(HOME)/sysroot
PREFIX?=/usr
LIBDIR?=$(PREFIX)/lib
INCLUDEDIR?=$(PREFIX)/include

ARCHDIR=arch/i386

include $(ARCHDIR)/make.config

WARNINGS:=  -Wall -Wextra -pedantic -Wshadow -Wpointer-arith -Wcast-align \
            -Wwrite-strings -Wmissing-declarations \
            -Wredundant-decls -Winline -Wno-long-long \
            -Wuninitialized -Wconversion

C_ONLY_FLAGS:=-std=c99
CFLAGS:=$(CFLAGS) -ffreestanding $(WARNINGS) $(ARCH_CFLAGS)
CPPFLAGS:=$(CPPFLAGS) -D__is_libc -Iinclude $(ARCH_CPPFLAGS)
LIBK_CFLAGS:=$(CFLAGS) $(KERNEL_ARCH_CFLAGS)
LIBK_CPPFLAGS:=$(CPPFLAGS) -D__is_libk $(KERNEL_ARCH_CPPFLAGS)

FREESRC:=$(patsubst ./%, %, $(shell find -name *.c -not -path arch))
FREEOBJS:=$(ARCH_FREEOBJS) $(FREESRC:.c=.o) 

HOSTEDOBJS=\
$(ARCH_HOSTEDOBJS) \

OBJS=\
$(FREEOBJS) \
$(HOSTEDOBJS) \

LIBK_OBJS=$(FREEOBJS:.o=.libk.o)

#BINARIES=libc.a libk.a # Not ready for libc yet.
BINARIES=libk.a

.PHONY: all clean install install-headers install-binairies
.SUFFIXES: .o .libk.o .c .S

all: $(BINARIES)

libc.a: $(OBJS)
	$(AR) rcs $@ $(OBJS)

libk.a: $(LIBK_OBJS)
	$(AR) rcs $@ $(LIBK_OBJS)

.c.o:
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS) $(C_ONLY_FLAGS)

.c.S:
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

.c.libk.o:
	$(CC) -MD -c $< -o $@ $(LIBK_CFLAGS) $(LIBK_CPPFLAGS)

.S.libk.o:
	$(CC) -MD -c $< -o $@ $(LIBK_CFLAGS) $(LIBK_CPPFLAGS)

clean:	
	@rm -f $(BINARIES) *.a
	@rm -f $(OBJS) $(LIBK_OBJS) *.o */*.o */*/*.o
	@rm -f $(OBJS:.o=.d) $(LIBK_OBJS:.o=.d) *.d */*.d */*/*.d

install: install-headers install-binairies

install-headers:
	@echo 'Install lib headers'
	@mkdir -p $(DESTDIR)$(INCLUDEDIR)
	@cp -R --preserve=timestamps include/. $(DESTDIR)$(INCLUDEDIR)/.

install-binairies: $(BINARIES)
	@echo 'Install lib'
	@mkdir -p $(DESTDIR)$(LIBDIR)
	@cp $(BINARIES) $(DESTDIR)$(LIBDIR)

-include $(OBJS:.o=.d)
-include $(LIBK_OBJS:.o=.d)
