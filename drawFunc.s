.section .text

.global calculateLM1
@int lm1 = -(hmap[x] + horizon_pos > res_Y / 2 ? res_Y / 2 : hmap[x] + horizon_pos)@
calculateLM1:
    @ r0 = hmap[x]
    @ r1 = horizon_pos
    @ r2 = res_Y

    mov r5, #2 @ r5 = 2
    add r3, r0, r1 @ r3 = hmap[x] + horizon_pos
    sdiv r4, r2, r5 @ r4 = res_Y/2

    cmp r3, r4 @ r3 > r4

    movgt r0, r4 @ r3 = res_Y/2

    movle r0, r3 @ r3 = hmap[x] + horizon_pos
    neg r0, r0

    bx lr

.global calculateLM2
@ int lm2 = (hmap[x] - horizon_pos > res_Y / 2 ? res_Y / 2 : hmap[x] - horizon_pos)@
calculateLM2:
    @ r0 = hmap[x]
    @ r1 = horizon_pos
    @ r2 = res_Y

    mov r5, #2 @ r5 = 2
    sub r3, r0, r1 @ r3 = hmap[x] - horizon_pos
    sdiv r4, r2, r5 @ r4 = res_Y / 2

    cmp r3, r4 @ r3 > r4

    movgt r0, r4 @ r0 = r4 (res_Y / 2)

    movle r0, r3 @ r0 = r3 (hmap[x] - horizon_pos)

    bx lr

.global calculateAngle
@ int ang = (int)(3600 + player.ang_h + (x - res_X / 2) * fov / res_X);
calculateAngle:
    @ Input parameters:
    @ r0 = player.ang_h
    @ r1 = x
    @ r2 = res_X
    @ r3 = fov
    mov r4, #2       
    sdiv r4, r2, r4   @ r4 = res_X / 2
    sub r5, r1, r4   @ r5 = x - res_X / 2
    mul r5, r3, r5   @ r5 = (x - res_X / 2) * fov
    sdiv r5, r5, r2   @ r5 = (x - res_X / 2) * fov / res_X
    mov r4, #3600     @ r4 = 3600
    add r0, r0, r4   @ r0 = player.ang_h + 3600
    add r0, r0, r5   @ r0 = player.ang_h + 3600 + (x - res_X / 2) * fov / res_X

    bx lr

.global mod
mod:
    @ Input: x0 = dividend (a)
    @        x1 = divisor  (b)
    @ Output: x0 = result (a % b)

    @ Check if divisor is zero (division by zero is undefined)
    beq handle_div_by_zero


    @ r = a / b
    sdiv r2, r0, r1
    
    @ r = r * b
    mul r2, r1, r2
    
    @ remainder = a - r
    sub r0, r0, r2
    
    bx lr

handle_div_by_zero:
    @ Handle division by zero as you see fit.
    @ For simplicity, we'll just return 0.
    mov r0, #0
    bx lr


@ .global calculate_crdy
@ @int crdy = 16 + (int)(14 * (y + horizon_pos) / hmap[x]);
@ calculate_crdy:
@     @ Input:  r0 = x
@     @         r1 = y
@     @         r2 = horizon_pos
@     @         r3 = address of hmap (pointer)
@     @ Output: r0 = crdy

@     ldr r4, [r3]           @ Load hmap[x] into r4 (assuming x is in r0)
@     add r1, r1, r2         @ r1 = y + horizon_pos
@     ldr r5, =14            @ Load the constant 14 into r5
@     mul r1, r1, r5         @ r1 = 14 * (y + horizon_pos)
@     add r0, r1, #16        @ r0 = 16 + (14 * (y + horizon_pos))
@     sdiv r0, r0, r4        @ r0 = (16 + 14 * (y + horizon_pos)) / hmap[x]

@     bx lr



