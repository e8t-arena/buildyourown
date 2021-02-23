#! /bin/sh
#
# boot.sh
# Copyright (C) 2021 lxy <lxy@lxy-mbp.local>
#
# Distributed under terms of the MIT license.
#


nasm -fbin bootsec.asm -o bootsec.bin 
qemu-system-x86_64 bootsec.bin
