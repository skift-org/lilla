#!/bin/bash

set -xue

# Path to clang and compiler flags
CC=clang++  # Ubuntu users: use CC=clang
CFLAGS="-std=gnu++2c -O2 -g3 -Wall -Wextra -Werror --target=riscv32-unknown-elf -fno-stack-protector -ffreestanding -nostdlib"

# Build the kernel
$CC $CFLAGS -Wl,-Tkernel.ld -Wl,-Map=kernel.map -o kernel.elf \
    src/kernel.cpp

# QEMU file path
QEMU=qemu-system-riscv32

# Start QEMU
$QEMU -machine virt -bios default -nographic -serial mon:stdio --no-reboot -kernel kernel.elf