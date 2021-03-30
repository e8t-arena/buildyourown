#
# Makefile
# Peter Lau, 2021-03-30 19:54
#
# vim:ft=make
#

all:
	@echo "Makefile needs your attention"

# 检查 ipl.bin 依赖文件 ipl.nas Makefile
ipl.bin : ipl.nas Makefile
	nasm -fbin ipl.nas -o ipl.bin
	# nasm -fbin ipl.nas -o ipl.bin ipl.lst

os.img : ipl.bin Makefile
	dd if=ipl.bin of=os.img count=1 bs=512
	dd if=/dev/zero of=os.img bs=512 seek=1 skip=1 count=2879

# 没有文件生成的指令

.PHONY: img asm run install clean 

img :
	make -r os.img

asm :
	make -r ipl.bin

run :
	make img
	qemu-system-i386 -drive format=raw,file=os.img

install :
	make img

clean :
	/bin/rm -f ipl.bin os.img
