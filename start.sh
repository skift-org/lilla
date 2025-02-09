#!/bin/bash

set -xue

# Path to clang and compiler flags
CXX=clang++  # Ubuntu users: use CC=clang
CFLAGS="-std=gnu++2c -O2 -g3 -Wall -Wextra -Werror --target=riscv32-unknown-elf -fno-stack-protector -ffreestanding -nostdlib"

AS=clang++
ASFLAGS="--target=riscv32"

# Build the kernel
$AS $ASFLAGS -c -o src/kernel.s.o src/kernel.s
$CXX $CFLAGS -c -o src/kernel.cpp.o src/kernel.cpp
$CXX $CFLAGS -c -o src/lib/libc.cpp.o src/lib/libc.cpp
$CXX $CFLAGS -Wl,-Tkernel.ld -Wl,-Map=kernel.map -o kernel.elf \
    src/kernel.s.o src/kernel.cpp.o src/lib/libc.cpp.o

# QEMU file path
QEMU=qemu-system-riscv32

# Start QEMU
$QEMU -machine virt -bios default -nographic -serial mon:stdio --no-reboot -kernel kernel.elf
