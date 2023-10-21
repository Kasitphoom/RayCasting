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
    mov r2, #0
    //cmp r1, #0
    //zero divisio error
    //beq 
    cmp r0, #0
    blt absolute1
    b moduloP2

absolute1:
    neg r0, r0
    mov r2, #1
    b moduloP2

moduloP2:
    sdiv r3, r0, r1
    mul r4, r3, r1
    cmp r2, #1
    beq absolute2
    b moduloP3

absolute2:
    neg r0, r0
    neg r4, r4
    b moduloP3

moduloP3:
    sub r0, r0, r4
    bx lr





