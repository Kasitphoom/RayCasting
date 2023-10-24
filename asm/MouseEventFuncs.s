.section .text
.global getPlayerHAngel


getPlayerHAngel:
    sub r3, r0, r1
    mov r4, #500   @Load immediate value 500 into r4
    mul r0, r3, r4
    sdiv r0, r0, r2
    bx lr

.global getPlayerVAngel

getPlayerVAngel:
    sub r3, r0, r1
    mov r4, #20    @Load immediate value 20 into r4
    mul r0, r3, r4
    sdiv r0, r0, r2
    bx lr

.global checkAdd360deg

checkAdd360deg:
    cmp r0, #3600
    blt true
    b false
true:
    mov r0, #3600
    bx lr
false:
    mov r0, #0
    bx lr

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





