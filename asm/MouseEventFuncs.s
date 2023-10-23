.section __TEXT, __text
.align 4
.global _getPlayerHAngel
.global _getPlayerVAngel
.global _checkAdd360deg


_getPlayerHAngel:
    sub w3, w0, w1
    mov w4, #500   ;Load immediate value 500 into r4
    mul w0, w3, w4
    sdiv w0, w0, w2
    ret

_getPlayerVAngel:
    sub w3, w0, w1
    mov w4, #20    ;Load immediate value 20 into w4
    mul w0, w3, w4
    sdiv w0, w0, w2
    ret

_checkAdd360deg:
    cmp w0, #3600
    blt true
    b false
true:
    mov w0, #3600
    ret
false:
    mov w0, #0
    ret
