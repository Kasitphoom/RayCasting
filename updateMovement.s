.section .text
.global mul_double
.global add_double
.global sub_double
.global div_double
.global bgt

mul_double:
    vmul.f64 d0, d0, d1
    bx lr

add_double:
    vadd.f64 d0, d0, d1
    bx lr

sub_double:
    vsub.f64 d0, d0, d1
    bx lr

div_double:
    vdiv.f64 d0, d0, d1
    bx lr

bgt:
    cmp r0, r1
    blt bgt_true
    mov r0, #0
    bx lr


bgt_true:
    mov r0, #1
    bx lr

