; 引导系统后进入循环
; loop:
    ; jmp loop

mov ah, 0x0e ; 0x0e 表示 tty 模式 (终端模式) 只用设置一次
mov al, 'B'  ; 复制 al 保存 'B'
int 0x10     ; 输出到屏幕
mov al, 'l'
int 0x10
mov al, 'a'
int 0x10
mov al, 'd'
int 0x10
mov al, 'e'
int 0x10

jmp $

times 510 - ($-$$) db 0
dw 0xaa55
