print_hex:
    pusha
    mov cx, 0

hex_loop:
    cmp cx, 4
    je print_hex_end ; jump if equal

    mov ax, dx
    and ax, 0x000f
    add al, 0x30
    cmp al, 0x39
    jle print_hex_next ; jump if less or equal
    add al, 7
    ; 默认顺序执行 next ?

print_hex_next:
    mov bx, HEX_OUT+5
    sub bx, cx
    mov [bx], al
    ror dx, 4

    add cx, 1
    jmp hex_loop

print_hex_end:
    mov bx, HEX_OUT
    call print

    popa
    ret

HEX_OUT:
    db '0x0000', 0
