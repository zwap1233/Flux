#!/bin/sh
set -e
. ./clean.sh
. ./build.sh

mkdir -p $ISODIR
mkdir -p $ISODIR/boot
mkdir -p $ISODIR/boot/grub

cp $SYSROOT/boot/$KERNFILE $ISODIR/boot/$KERNFILE
cat > $ISODIR/boot/grub/grub.cfg << EOF
menuentry "$OSNAME" {
	multiboot /boot/$KERNFILE
	boot
}
EOF
grub-mkrescue -o $ISOFILE $ISODIR
