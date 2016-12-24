SYSTEM_HEADER_PROJECTS="libk system"
PROJECTS="libk system"

export MAKE=${MAKE:-make}
export HOST=${HOST:-$(./default-host.sh)}

export AR=${HOST}-ar
export AS=${HOST}-as
export CC=${HOST}-g++

export PREFIX=/usr
export EXEC_PREFIX=$PREFIX
export BOOTDIR=/boot
export LIBDIR=$EXEC_PREFIX/lib
export INCLUDEDIR=$PREFIX/include

export CFLAGS='-O2 -g'
export CPPFLAGS='-fno-exceptions -fno-rtti'

#inbuild vars
export OSNAME=FluxOS
export KERNFILE=$OSNAME.bin
export ISODIR=isodir
export ISOFILE=$OSNAME.iso

# Configure the cross-compiler to use the desired system root.
export SYSROOT="$(pwd)sysroot"
export CC="$CC --sysroot=$SYSROOT"

# Work around that the -elf gcc targets doesn't have a system include directory
# because it was configured with --without-headers rather than --with-sysroot.
if echo "$HOST" | grep -Eq -- '-elf($|-)'; then
  export CC="$CC -isystem=$INCLUDEDIR"
fi
