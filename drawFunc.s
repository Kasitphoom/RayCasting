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
    mul r5, r5, r3   @ r5 = (x - res_X / 2) * fov
    sdiv r5, r5, r2   @ r5 = (x - res_X / 2) * fov / res_X
    mov r4, #3600     @ r4 = 3600
    add r0, r0, r4   @ r0 = player.ang_h + 3600
    add r0, r0, r5   @ r0 = player.ang_h + 3600 + (x - res_X / 2) * fov / res_X

    bx lr






