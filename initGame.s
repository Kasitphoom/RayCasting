.section .text
.global compareZero

compareZero:
    cmp r0, #0         @ Compare r0 with 0
    moveq r0, #1       @ If equal, set r0 to 1 (true)
    movne r0, #0       @ If not equal, set r0 to 0 (false)
    bx lr

.global greaterThan
greaterThan:
    cmp r0, r1       @ Compare r0 and r1
    bgt greater_than @ Branch if r0 > r1
    mov r0, #0       @ Set r0 to 0 (false)
    bx lr

greater_than:
    mov r0, #1       @ Set r0 to 1 (true)
    bx lr

.global GTE
GTE:
    cmp r0, r1       @ Compare r0 and r1
    bge greater_equal @ Branch if r0 >= r1
    mov r0, #0       @ Set r0 to 0 (false)
    bx lr

greater_equal:
    mov r0, #1       @ Set r0 to 1 (true)
    bx lr

.global lessThan
lessThan:
    cmp r0, r1       @ Compare r0 and r1
    blt less_than    @ Branch if r0 < r1
    mov r0, #0       @ Set r0 to 0 (false)
    bx lr

less_than:
    mov r0, #1       @ Set r0 to 1 (true)
    bx lr

.global LTE
LTE:
    cmp r0, r1       @ Compare r0 and r1
    ble less_equal   @ Branch if r0 <= r1
    mov r0, #0       @ Set r0 to 0 (false)
    bx lr

less_equal:
    mov r0, #1       @ Set r0 to 1 (true)
    bx lr

.global EQ
EQ:
    cmp r0, r1       @ Compare r0 and r1
    beq equal        @ Branch if r0 == r1
    mov r0, #0       @ Set r0 to 0 (false)
    bx lr

equal:
    mov r0, #1       @ Set r0 to 1 (true)
    bx lr

.global logicalOR
logicalOR:
    cmp r0, #0      @ Compare the value in r0 with 0
    bne or_done     @ If r0 is not equal to 0, skip the OR operation
    cmp r1, #0      @ Compare the value in r1 with 0
    bne or_done     @ If r1 is not equal to 0, skip the OR operation
    mov r0, #0      @ If both r0 and r1 are 0, set r0 to 0 (false)
    b or_end
or_done:
    mov r0, #1      @ If at least one of r0 or r1 is not 0, set r0 to 1 (true)
    bx lr
or_end:
    bx lr

.global logicalAND
logicalAND:
    cmp r0, #0      @ Compare the value in r0 with 0
    beq and_done     @ If r0 is equal to 0, skip the AND operation
    cmp r1, #0      @ Compare the value in r1 with 0
    beq and_done     @ If r1 is equal to 0, skip the AND operation
    mov r0, #1      @ If both r0 and r1 are not 0, set r0 to 1 (true)
    b and_end
and_done:
    mov r0, #0      @ If at least one of r0 or r1 is 0, set r0 to 0 (false)
and_end:
    bx lr

.global logicalXOR
logicalXOR:
    cmp r0, r1      @ Compare the value in r0 with r1
    moveq r0, #0    @ If equal, set r0 to 0 (false)
    movne r0, #1    @ If not equal, set r0 to 1 (true)
    bx lr



.global mul_int
.global add_int
.global sub_int
.global div_int

mul_int:
    mul r0, r0, r1
    bx lr

add_int:
    add r0, r0, r1
    bx lr

sub_int:
    sub r0, r0, r1
    bx lr

div_int:
    sdiv r0, r0, r1
    bx lr

