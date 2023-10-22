@ lowest_time(&r_y, &t1, &r_x, &t2, &r_ix, &r_iy, &r_vx, &r_vy, &r_ivx, &r_ivy, &r_dist);
@ r_y = r0
@ t1 = r1
@ r_x = r2
@ t2 = r3
@ r_ix = [sp, #4]
@ r_iy = [sp, #8]
@ r_vx = [sp, #12]
@ r_vy = [sp, #16]
@ r_ivx = [sp, #20]
@ r_ivy = [sp, #24]
@ r_dist = [sp, #28]
.data
pattern: .asciz "The lowest time is [ASM] %p\n"

.text
.global lowest_time
.extern printf
lowest_time:
    push {lr}

    @ check if t1 < t2
    vmov.f32 s0, r1
    vmov.f32 s1, r3

    vcvt.f64.f32 d0, s0
    vcvt.f64.f32 d1, s1

    vcmp.f64 d0, d1
    vmrs APSR_nzcv, fpscr
    bge t1_more_than_t2

    @ t1 < t2
    @ r_y += r_vy * t1

    vmov.f32 s0, r0
    vldr.f32 s1, [sp, #16]
    vmov.f32 s2, r1

    vcvt.f64.f32 d0, s0
    vcvt.f64.f32 d1, s1
    vcvt.f64.f32 d2, s2

    @ store back to 

    pop {pc}
    bx lr
