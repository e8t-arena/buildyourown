; 在 C 中调用 asm 
; TAB=2

; [FORMAT "WCOFF"]
; [INSTRSET "i486p"] ; 指令到 486 为止

[bits 32]

; GLOBAL io_hlt
; GLOBAL write_mem8

; [FILE "asmfunc.nas"]

GLOBAL io_hlt, write_mem8
GLOBAL io_cli, io_sti, io_stihlt
GLOBAL io_in8, i_in16, io_in32
GLOBAL io_out8, i_out16, io_out32
GLOBAL io_load_eflags, io_store_eflags

_start:
    [extern OSMain]
    call OSMain

io_hlt:
  HLT
  RET

write_mem8:  ; void write_mem8(int addr, int data)
  mov ecx, [esp+4] ; 存放地址
  mov al, [esp+8]  ; 存放 data
  mov [ecx], al 
  ret

io_cli:
  CLI
  RET

io_sti:
  STI
  RET

io_stihlt:
  CLI
  HLT
  RET

io_in8:           ; int io_in8(int port)
  MOV EDX,[esp+4] ; port 值
  MOV EAX,0
  IN AL, DX
  RET

io_in16:
  MOV EDX,[esp+4] ; port 值
  MOV EAX,0
  IN AX, DX
  RET

io_in32:
  MOV EDX,[esp+4] ; port 值 函数第一个参数
  IN EAX, DX
  RET

io_out8:            ; void io_out8(int port, int data)
  MOV EDX, [esp+4]  ; port
  MOV AL, [esp+8]   ; data
  OUT DX, AL
  RET

io_out16:
  MOV EDX, [esp+4]  ; port
  MOV AX, [esp+8]   ; data
  OUT DX, AX
  RET

io_out32:
  MOV EDX, [esp+4]  ; port
  MOV EAX, [esp+8]  ; data
  OUT DX, EAX
  RET

io_load_eflags: ; read eflags
  PUSHFD ; push eflags     
  POP EAX
  RET

io_store_eflags: ; write eflags
  MOV EAX, [esp+4]
  PUSH EAX
  POPFD ; pop eflags
  RET

