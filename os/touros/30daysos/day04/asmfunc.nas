; 在 C 中调用 asm 

[bits 32]

; GLOBAL io_hlt
; GLOBAL write_mem8

GLOBAL io_hlt, write_mem8

_start:
    [extern HariMain]
    call HariMain

io_hlt:
  HLT
  RET

write_mem8:  ; void write_mem8(int addr, int data)
  mov ecx, [esp+4] ; 存放地址
  mov al, [esp+8]  ; 存放 data
  mov [ecx], al 
  ret
