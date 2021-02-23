# tourOS

## 0x00

https://www.52pojie.cn/thread-1369484-1-1.html 你没看错：动手开发 GUI 简单操作系统（一）

### ch1

安装 qemu nasm 

brew install qemu nasm

- 加载启动扇区

为了检测磁盘是可启动的，BIOS会检测第511和512字节是否为十六进制AA55 : 0xaa55

boot.sh

nasm -fbin bootsec.asm -o bootsec.bin 
qemu-system-x86_64 bootsec.bin

sh ./boot.sh 启动

- 输出文字到屏幕

中断，操作系统停下当前任务，响应指令

输出字符需要 int 0x10 中断，控制屏幕输出

int 0x10 有两个参数

  ax 寄存器

  低位 al 为输出字符

  高位 ah 为输出模式的指示符


# realOS
