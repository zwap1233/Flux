#!/bin/sh
set -e
. ./headers.sh

for PROJECT in $PROJECTS; do
  (cd $PROJECT && DESTDIR="$SYSROOT" $MAKE install)
done

grub-file --is-x86-multiboot system/$KERNFILE
if [ "$?" -eq "1" ] ; then
	echo 'ERROR: Kernel file is not multiboot'
fi