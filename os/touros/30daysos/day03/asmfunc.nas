; 在 C 中调用 asm 

; 使用 nasm 调整部分代码
; [FORMAT "WCOFF"]
; [bits 32]
; 
; [FILE "asmfunc.nas"]
; 
; GLOBAL _io_hlt
; 
; [SECTION .text]
; _io_hlt:
;   HLT
;   RET

[bits 32]

GLOBAL io_hlt

_start:
    [extern HariMain]
    call HariMain

io_hlt: ; void io_hlt(void);
  HLT
  RET
