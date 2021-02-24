; https://www.tutorialspoint.com/assembly_programming/assembly_registers.htm# Assembly - Registers - Tutorialspoint

section .text
  global _start ; must be declared for linker

; 首字母大写作为变量
_start:
  mov rdx, Len
  ; mov rcx, Msg
  ; mov rbx, 1
  ; mov rax, 4
  mov rdi, 1
  mov rsi, Msg
  mov       rax, 0x02000004
  ;int 0x80
  syscall

  mov rdx, 9
  ; mov rcx, Stars
  mov rdi, 1
  mov rsi, Stars
  ; mov rax, 4
  mov       rax, 0x02000004
  ;int 0x80
  syscall

  ; mov rax, 1
  mov       rax, 0x02000001
  ;int 0x80
  xor rdi, rdi
  syscall

; 自定义变量
section .data
Msg db 'Displaying 9 stars', 0xa ;a message
Len equ $ - Msg ; length of Msg
Stars times 9 db '*'

; https://cs.lmu.edu/~ray/notes/nasmtutorial/ NASM Tutoriak 
; 组成一次 system call (sys_write)
;   mov rax, 4
;   int 0x80
; mov rax, 1 (sys_exit)
