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

- 打印空行 \n = 0x0a 0x0d

- 打印地址: 地址为十六进制

指定字符转 ASCII。传入参数不是字符而是地址

'0' - '9' 数字加 0x30 为对应 ASCII

'A' - 'F' 字符加 0x40

  0x40 = 64

  'A' 当作 1 'F' 当作 6

  +0x40 后得到对应 ASCII

  ’A' -> 65

mov cx, 0 cx 记录循环次数

地址: 8 bits, 4 bytes
https://en.wikipedia.org/wiki/Memory_address Memory address - Wikipedia

cmp cx, 4
je print_hex_end -> 过程标签全局唯一
  cx 为 4 就停止打印


mov ax, dx
  ax 保存地址, dx 是地址参数

and ax, 0x000f
  位与操作 保留最后一位 0x1234 -> 0x0004
  与操作后再赋值给 al

add al, 0x30
  0x0004 保存在低位 al
  al+0x30 后再赋值给 al

cmp al, 0x39
  判读读到的是字符还是数字

  0x40 = 64 = '@'
  0x39 = 63
  0x38 = 62

  0x41 = 65 = 'A'

  0x1234 -> 0x0004 -> (+30) 0x0034 -> (<= 0x0039) ->
  52 -> 即 '4' 的 ASCII 码

  iex(1)> Integer.parse("0x0034", 16)
  {0, "x0034"}

  0x123A -> 0x000A -> 0x003A -> (> 0x0039) -> (+7) -> 0x41
  ->  '65' -> 'A'

jle hex_num
  如果 al <= 0x39 (63) 就表示字符为 '0' - '9' 跳转到 hex_num 标签
  否则 表示字符为 'A' - 'F'
    如果字符为 'A' 那此时 al 保存值 0x3A (58) +7 = 65 = 'A'

add al, 7
  按照字母打印
  例如 'A'
  and ax, 0x000F
  -> 0x000A -> (+0x0030) -> 0x003A -> 3*16^1+10 -> 58 -> 58+7 -> 65 -> 'A'

hex_num 打印数字字符串

mov bx, HEX_OUT + 5
  基地址 + 5 (4 位字符 + 0x000)

sub bx, cx
  bx 值为 基地址+5
  cx 为循环计数
  计算出需要赋值的地址位置

  bx
  _ _ _ _ _

  (+5)

  al: '1234'

  bx: '4321'

  _ _ _ _ _ (base+5)
  _ _ _ _ _ (cx 为 0)

  _ _ _ _ _ (base+5-1)
  _ _ _ _ _ (cx 为 1)

mov [bx], al

  [bx] 表示 bx 的地址内容，而不是储存内容
  并将 al 赋值给这个地址

ror dx, 4
  遍历 dx 目的是每次只处理一个字符，即末尾的那个，
  因为通过 0x1234 and 0x000F 只保留了末尾
  0x1234 "1234"

add cx, 1
  遍历循环+1

call print
  调用 print 过程

HEX_OUT:
  db '0x0000', 0 地址字符串+‘0x00'
  DB - Define Byte. 8 bits

https://stackoverflow.com/questions/10168743/which-variable-size-to-use-db-dw-dd-with-x86-assembly Which variable size to use (db, dw, dd) with x86 assembly? - Stack Overflow

%include 是 %get 的别名

直接不做修改复制文件到该位置

print.asm:49: error: parser: instruction expected

这意味着，减去include的行数，就是源文件中出现错误的地方。

- 读取磁盘


https://www.keil.com/support/man/docs/armasm/armasm_dom1361289891242.htm Assembler User Guide: ROR
https://www.keil.com/support/man/docs/armasm/armasm_dom1361290015482.htm Assembler User Guide: GET or INCLUDE

# realOS
