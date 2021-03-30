#! /bin/sh
#
# boot.sh
# Copyright (C) 2021 Peter Lau <superpeterlau@outlook.com>
#
# Distributed under terms of the MIT license.
#

# nasm -fbin hello2.nas -o bootsec.bin 
# qemu-system-x86_64 bootsec.bin

if [[ -n $1 ]]; then
  nasm -fbin $1 -o bootloader
  dd if=bootloader of=os.img count=1 bs=512
  dd if=/dev/zero of=os.img bs=512 seek=1 skip=1 count=2879
  qemu-system-i386 -drive format=raw,file=os.img
else
  echo "Choose asm file"
  exit 1
fi
  
