.global modulo
modulo:
    cmp r1, #0
    beq handle_div_by_zero

    sdiv r2, r0, r1
    mul r2, r2, r1
    sub r0, r0, r2

    bx lr

handle_div_by_zero:
    mov r0, #0
    bx lr