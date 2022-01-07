#
# Makefile
# Peter Lau, 2021-03-31 13:04
#

all:
	@echo "Makefile"


# vim:ft=make

C_SOURCES = $(wildcard *.c)

OBJ = ${C_SOURCES:.c=.o}

CC = /usr/local/bin/i386-elf-gcc
GDB = /usr/local/bin/i386-elf-gdb
BIN2OBJ = /usr/local/Cellar/i386-elf-binutils/2.37_1/bin/i386-elf-objcopy
# LD = /usr/local/bin/i386-elf-ld
LD = /usr/local/Cellar/i386-elf-binutils/2.37_1/bin/i386-elf-ld
ASMC = /usr/local/bin/nasm

CFLAGS = -g -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs \
		 -Wall -Wextra

INC_DIR = ../includes/golibc
LIB_OPT = -lgolibc

DISK=disk2
TINY_DISK=/dev/$(DISK)
DISK_UTILS=/usr/bin/hdiutil

C_SOURCES = $(wildcard *.c)
OBJ = ${C_SOURCES:.c=.o}
OBJ += asmfunc.o hankaku.o

# compile fonts

# hankaku.bin: hankaku.txt
# 	${MAKEFONT} $< $@

hankaku_need_to_redefine_symbol_name.o: ../fonts/hankaku.bin
	${BIN2OBJ} -I binary $< -O elf32-i386 $@ -B i386

hankaku.o: hankaku_need_to_redefine_symbol_name.o
	${BIN2OBJ} -I elf32-i386 $< -O elf32-i386 $@ --redefine-sym _binary____fonts_hankaku_bin_start=hankaku

os.bin: ${OBJ}
	# ${LD} -o $@ -Ttext 0x00280000 $^ --oformat binary
	${LD} -m elf_i386 -o $@ -Ttext 0x00280000 $^ --oformat binary -L ${INC_DIR} ${LIB_OPT}

asmhead.bin: asmhead.nas
	${ASMC} $< -f bin -o $@

ipl.bin: ipl.nas
	${ASMC} $< -f bin -o $@

%.o: %.c ${HEADERS}
	${CC} ${CFLAGS} -I${INC_DIR} -ffreestanding -c $< -o $@

%.o: %.nas
	${ASMC} $< -f elf -o $@

ipl.img : ipl.bin Makefile
	dd if=ipl.bin of=ipl.img count=1 bs=512
	dd if=/dev/zero of=ipl.img bs=512 seek=1 skip=1 count=2879

os.img : ipl.img asmhead.bin os.bin
	cat asmhead.bin os.bin > os.sys
	# copy os.bin to os.img (macOS)
	# TINY_DISK := $(hdiutil attach -blocksize 512 -noverify -nomount os.img)
	$(DISK_UTILS) detach $(DISK); $(DISK_UTILS) attach -blocksize 512 -noverify -nomount ipl.img
	# hdiutils 输出为
	# mkdir -p /tmp/tinyos && mount -t msdos $(TINY_DISK) /tmp/tinyos && cp <(cat asmhead.bin os.sys) /tmp/tinyos/. && umount $(TINY_DISK)
	mkdir -p /tmp/tinyos && mount -t msdos $(TINY_DISK) /tmp/tinyos && cp os.sys /tmp/tinyos/. && umount $(TINY_DISK)
	dd if=$(TINY_DISK) of=os.img bs=512
	$(DISK_UTILS) detach $(DISK)

debug.bin : ipl.bin asmhead.bin os.bin
	cat $^ > debug.bin

.PHONY: build, img, runimg, run, clean

build:
	@echo "Building"
	make -r ipl.bin asmhead.bin os.bin

img:
	@echo "Make Image"
	make -r os.img

bin:
	echo "Make Bin"
	make -r debug.bin

runimg:
	@echo "Run Image"
	qemu-system-i386 -drive format=raw,file=os.img,index=0,if=floppy

runbin:
	@echo "Run Bin"
	qemu-system-i386 -fda debug.bin

rundebug:
	make build
	make bin
	make runbin

run:
	make build
	make img 
	make runimg

clean:
	/bin/rm -rf *.o *.bin *.img *.sys *.out
