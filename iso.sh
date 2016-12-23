#!/bin/sh
set -e
. ./build.sh

mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub

KERNFILE=$1
OSNAME=$2
ISO=$3

cp sysroot/boot/NoletOS.bin isodir/boot/NoletOS.bin
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "NoletOS" {
	multiboot /boot/NoletOS.bin
}
EOF
grub-mkrescue -o NoletOS.iso isodir
