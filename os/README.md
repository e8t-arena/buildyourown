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

- 打印功能（函数)

需要打印字符串和地址

"Blade" -> 'B' 'l' 'a' 'd' 'e' '0x00' 在汇编中用栈保存

pusha 
  将所有寄存器内容压入栈

mov al, [bx] 
  bx 表示字符串首位

cmp al, 0 
  比较 al 和 0 相等表示遇到 0x00 字符串结束

je done 
  如果相等则 jump 到 done 标签处

mov ah, 0x0e
int 0x10
  al 有内容，直接打印

add bx, 1
  栈地址加 1 地址后移一位，开始读取下一个字符

popa
  将全部栈弹出 (内存释放?)

ret
  返回 #主程序

保存在 print.asm

打印空行 \n = 0x0a 0x0d

打印地址: 地址为十六进制

指定字符转 ASCII。传入参数不是字符而是地址

'0' - '9' 数字加 0x30 为对应 ASCII

'A' - 'F' 字符加 0x

# realOS
