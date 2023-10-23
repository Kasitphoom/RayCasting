.section .text
.global pressWx

pressWx:
    cmp r1, r2
    beq trueWx
    b falseWx
trueWx:
    mov r0, #0
    bx lr
falseWx:
    bx lr

.global pressWy

pressWy:
    cmp r1, r2
    beq trueWy
    b falseWy
trueWy:
    mov r0, #0
    bx lr
falseWy:
    bx lr
