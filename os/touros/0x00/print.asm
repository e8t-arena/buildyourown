print:
    pusha

start:
    mov al, [bx]
    cmp al, 0
    je end

    mov ah, 0x0e
    init 0x10

    add bx, 1
    jmp start

end:
    popa
    ret

print_nl: ; 打印空行
  pusha

  mov ah, 0x0e
  mov a1, 0x0a ; \n = 0x0a 0x0d
  int 0x10
  mov a1, 0x0d
  int 0x10

  popa
  ret
