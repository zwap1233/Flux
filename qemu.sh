#!/bin/sh
set -e
. ./clean.sh
. ./build.sh

qemu-system-$(./target-triplet-to-arch.sh $HOST) -kernel system/$KERNFILE
