#!/bin/sh
make

HOST=i686-elf
KERNFILE=Flux.bin

if echo $HOST | grep -Eq 'i[[:digit:]]86-'; then
    HOSTARCH=i386
else 
    echo $HOST
    grep -Eo '^[[:alnum:]_]*'
    HOSTARCH=$?
fi

echo $HOSTARCH
qemu-system-$HOSTARCH -kernel sysroot/boot/$KERNFILE
