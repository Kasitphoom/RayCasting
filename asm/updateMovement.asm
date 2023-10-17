#.global _compute_dx
#.global _compute_dy
.global _updatePlayerPositionAndVelocity

.text
.align 4


# compute_dx(double accel, int ang_h, double* sintab) 
/*
_compute_dx:
    stp x29, x30, [sp, -64]!      ; Store pair (frame pointer and link register) and prepare the stack
    mov x29, sp                   ; Set frame pointer

    mov x3, #3599                 ; Load 3599 into x3

    and x1, x1, x3                ; Compute (int)player.ang_h % 3600
    ldr d1, [x2, x1, lsl #3]      ; Load corresponding value from sintab

    fmul d0, d0, d1               ; dx = player.accel * sintab[(int)player.ang_h % 3600]
    
    ldp x29, x30, [sp], 64        ; Restore stack and pair
    ret

# compute_dy(double accel, int ang_h, double* sintab)
_compute_dy:
    stp x29, x30, [sp, -64]!      ; Store pair and prepare the stack
    mov x29, sp                   ; Set frame pointer

    mov x3, #3599                 ; Load 3599 into x3

    add x1, x1, #900              ; Compute ((int)player.ang_h + 900) % 3600
    and x1, x1, x3
    ldr d1, [x2, x1, lsl #3]      ; Load corresponding value from sintab

    fmul d0, d0, d1               ; dy = player.accel * sintab[((int)player.ang_h + 900) % 3600]

    ldp x29, x30, [sp], 64        ; Restore stack and pair
    ret
*/

_no_x_collision:
    ; Check y-axis collision
    fadd d10, d5, d7    ; d10 = player.y + player.vy
    fcvtzs wzr, d10     ; Convert to integer
    add x9, x6, x9
    ldr x9, [x9]
    ldr w9, [x9, x0, lsl #2] ; w9 = map[int(player.y + player.vy)][int(player.x)]
    and w9, w9, #255
    cmp w9, #0
    b.le _no_y_collision

    fmov d9, #2.0
    fdiv d11, d7, d9
    fneg d7, d11

_no_y_collision:
    ; Update player position
    fadd d4, d4, d6 ; player.x += player.vx
    fadd d5, d5, d7 ; player.y += player.vy

    ; Apply friction
    fmov d9, #1.0
    fsub d12, d9, d0
    fmul d6, d6, d12   ; player.vx *= (1 - player.friction)
    fmul d7, d7, d12   ; player.vy *= (1 - player.friction)

    ; Store updated values back
    str d4, [x0]
    str d5, [x1]
    str d6, [x2]
    str d7, [x3]

    ldp x29, x30, [sp], 96 ; Restore stack and pair
    ret


_updatePlayerPositionAndVelocity:
    stp x29, x30, [sp, -96]! ; Store pair and prepare stack
    mov x29, sp             ; Set frame pointer

    ; Parameters:
    ; x0 = pointer to player.x
    ; x1 = pointer to player.y
    ; x2 = pointer to player.vx
    ; x3 = pointer to player.vy
    ; d0 = player.friction
    ; x6 = pointer to map

    ; Load player values
    ldr d4, [x0] ; d4 = player.x
    ldr d5, [x1] ; d5 = player.y
    ldr d6, [x2] ; d6 = player.vx
    ldr d7, [x3] ; d7 = player.vy

    ; Check x-axis collision
    fadd d8, d4, d6     ; d8 = player.x + player.vx
    fcvtzs wzr, d8      ; Convert to integer
    add x8, x8, x6
    ldr x8, [x8]
    ldr w8, [x8, x1, lsl #2] ; w8 = map[int(player.x + player.vx)][int(player.y)]
    and w8, w8, #255
    cmp w8, #0
    b.le _no_x_collision

    fmov d9, #2.0
    fdiv d6, d6, d9
    fneg d6, d9



