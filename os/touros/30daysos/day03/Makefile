#
# Makefile
# Peter Lau, 2021-03-30 19:54
#
# vim:ft=make
#

TINY_DISK=/dev/disk3
DISK_UTILS=/usr/bin/hdiutil

all:
	@echo "Makefile"

# 检查 ipl.bin 依赖文件 ipl.nas Makefile
ipl.bin : ipl.nas Makefile
	nasm -fbin ipl.nas -o ipl.bin
	# nasm -fbin ipl.nas -o ipl.bin ipl.lst

os.bin : os.nas Makefile
	nasm -fbin os.nas -o os.bin

ipl.img : ipl.bin Makefile
	dd if=ipl.bin of=ipl.img count=1 bs=512
	dd if=/dev/zero of=ipl.img bs=512 seek=1 skip=1 count=2879

os.img : ipl.img os.bin
	# copy os.bin to os.img (macOS)
	# TINY_DISK := $(hdiutil attach -blocksize 512 -noverify -nomount os.img)
	$(DISK_UTILS) detach disk3; $(DISK_UTILS) attach -blocksize 512 -noverify -nomount ipl.img
	# hdiutils 输出为
	mkdir -p /tmp/tinyos && mount -t msdos $(TINY_DISK) /tmp/tinyos && cp os.bin /tmp/tinyos/. && umount $(TINY_DISK)
	dd if=$(TINY_DISK) of=os.img bs=512
	$(DISK_UTILS) detach disk3

# 没有文件生成的指令

.PHONY: ipl img asm run install clean

ipl :
	make -r ipl.img

img :
	make -r os.img

asm :
	make -r ipl.bin

runimg :
	# qemu-system-i386 -drive format=raw,file=os.img
	# FAT12 软盘启动模式
	# qemu-system-i386 -fda -drive format=raw,file=os.img
	qemu-system-i386 -drive format=raw,file=os.img,index=0,if=floppy

run :
	make clean
	make img
	make runimg

# install :
# 	make img

clean :
	/bin/rm -f *.bin *.img
