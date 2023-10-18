.section __TEXT, __text
.align 4

.global _getPlayerHAngel
_getPlayerHAngel:
    sub x3, x0, x1
    mov x4, #500
    mul x0, x3, x4
    sdiv x0, x0, x2
    ret

.global _getPlayerVAngel
_getPlayerVAngel:
    sub x3, x0, x1
    mov x4, #20
    mul x0, x3, x4
    sdiv x0, x0, x2
    ret

.global _checkAdd360deg
_checkAdd360deg:
    cmp x0, #3600
    b.lt true_label
    b false_label
true_label:
    mov x0, #3600
    ret
false_label:
    mov x0, #0
    ret
