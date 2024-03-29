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
# LD = /usr/local/bin/i386-elf-ld
LD = /usr/local/Cellar/i386-elf-binutils/2.37_1/bin/i386-elf-ld
ASMC = /usr/local/bin/nasm

CFLAGS = -g -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs \
		 -Wall -Wextra

DISK=disk2
TINY_DISK=/dev/$(DISK)
DISK_UTILS=/usr/bin/hdiutil

os.bin: asmfunc.o bootpack.o
	${LD} -m elf_i386 -o $@ -Ttext 0x00280000 $^ --oformat binary

asmhead.bin: asmhead.nas
	${ASMC} $< -f bin -o $@

ipl.bin: ipl.nas
	${ASMC} $< -f bin -o $@

%.o: %.c ${HEADERS}
	${CC} ${CFLAGS} -ffreestanding -c $< -o $@

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

.PHONY: build, img, runimg, run, clean

build:
	@echo "Building"
	make -r ipl.bin asmhead.bin os.bin

img:
	@echo "Make Image"
	make -r os.img

runimg:
	@echo "Run Image"
	qemu-system-i386 -drive format=raw,file=os.img,index=0,if=floppy

run:
	make build
	make img 
	make runimg

clean:
	/bin/rm -rf *.o *.bin *.img *.sys
