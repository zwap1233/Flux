
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

C_ONLY_FLAGS:=-std=c99
CFLAGS:=$(CFLAGS) -ffreestanding $(WARNINGS) $(KERNEL_ARCH_CFLAGS)
CPPFLAGS:=$(CPPFLAGS) -D__is_kernel -Iinclude $(KERNEL_ARCH_CPPFLAGS)
LDFLAGS:=$(LDFLAGS) $(KERNEL_ARCH_LDFLAGS)
LIBS:=$(LIBS) -nostdlib -lk -lgcc $(KERNEL_ARCH_LIBS)

KERNFILE?=Flux.bin
#Backup for linker, should be set in arch .config file
LINKER?=linker.ld

C_OBJS:=$(shell find kernel -name *.c)
CPP_OBJS:=$(shell find kernel -name *.cpp)
KERNEL_OBJS:=$(C_OBJS:.c=.o) $(CPP_OBJS:.cpp=.o) $(KERNEL_ARCH_OBJS)

OBJS=\
$(ARCHDIR)/crti.o \
$(ARCHDIR)/crtbegin.o \
$(KERNEL_OBJS) \
$(ARCHDIR)/crtend.o \
$(ARCHDIR)/crtn.o \

LINK_LIST=\
$(LDFLAGS) \
$(ARCHDIR)/crti.o \
$(ARCHDIR)/crtbegin.o \
$(KERNEL_OBJS) \
$(LIBS) \
$(ARCHDIR)/crtend.o \
$(ARCHDIR)/crtn.o \

.PHONY: all clean install install-headers install-kernel
.SUFFIXES: .o .c .cpp .S

all: $(KERNFILE)

$(KERNFILE): $(OBJS) $(ARCHDIR)/$(LINKER)
	$(CC) -T $(ARCHDIR)/$(LINKER) -o $@ $(CFLAGS) $(LINK_LIST)

$(ARCHDIR)/crtbegin.o $(ARCHDIR)/crtend.o:
	@OBJ=`$(CC) $(CFLAGS) $(LDFLAGS) -print-file-name=$(@F)` && cp "$$OBJ" $@

.c.o:
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS) $(C_ONLY_FLAGS)

.cpp.o:
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

.S.o:
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

clean:
	@rm -f $(KERNFILE)
	@rm -f $(OBJS) *.o */*.o */*/*.o
	@rm -f $(OBJS:.o=.d) *.d */*.d */*/*.d

install: install-headers install-kernel

install-headers:
	@mkdir -p $(DESTDIR)$(INCLUDEDIR)
	@cp -R --preserve=timestamps include/. $(DESTDIR)$(INCLUDEDIR)/.

install-kernel: $(KERNFILE)
	@mkdir -p $(DESTDIR)$(BOOTDIR)
	@cp $(KERNFILE) $(DESTDIR)$(BOOTDIR)

-include $(OBJS:.o=.d)
