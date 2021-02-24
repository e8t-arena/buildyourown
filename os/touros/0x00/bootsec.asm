; 引导系统后进入循环
; loop:
    ; jmp loop

; mov ah, 0x0e ; 0x0e 表示 tty 模式 (终端模式) 只用设置一次
; mov al, 'B'  ; 复制 al 保存 'B'
; int 0x10     ; 输出到屏幕
; mov al, 'l'
; int 0x10
; mov al, 'a'
; int 0x10
; mov al, 'd'
; int 0x10
; mov al, 'e'
; int 0x10
; 
; jmp $
; 
; times 510 - ($-$$) db 0
; dw 0xaa55

[org 0x7c00]

mov bx, GREETINGS
call print
call print_nl

mov dx, 0x4567
call print_hex
call print_nl

mov bx, SHUTDOWN
call print
call print_nl

jmp $ ; 挂起程序，进入无限循环 或 hlt

; 引入其他 asm 文件
; %include命令相当于把引用的文件直接替换到程序中，不做任何操作
%include "print.asm"
%include "print_hex.asm"

; 定义数据
GREETINGS:
  db 'Welcome to Blade', 0 ; 尾部 '0' 字节

SHUTDOWN:
  db 'Shutdown, bye', 0

times 510-($-$$) db 0
dw 0xaa55
