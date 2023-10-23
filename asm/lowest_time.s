@ lowest_time(&r_y, &t1, &r_x, &t2, &r_ix, &r_iy, &r_vx, &r_vy, &r_ivx, &r_ivy, &r_dist);
@ r_y = [sp, #4]
@ t1 = [sp, #8]
@ r_x = [sp, #12]
@ t2 = [sp, #16]
@ r_ix = [sp, #20]
@ r_iy = [sp, #24]
@ r_vx = [sp, #28]
@ r_vy = [sp, #32]
@ r_ivx = [sp, #36]
@ r_ivy = [sp, #40]
@ r_dist = [sp, #44]

.section .data
pattern: .asciz "The lowest time is [ASM] %p\n"

.balign 4
.section .text
.global lowest_time
.extern printf
lowest_time:
    push {r0, r1, r2, r3}
    push {lr}

    @ check if t1 < t2
    ldr r0, [sp, #8]   @ Load the address of t1
    ldr r1, [sp, #16]  @ Load the address of t2

    vldr.f64 d0, [r0]   @ Load t1 into s0
    vldr.f64 d1, [r1]   @ Load t2 into s1

    @ vcmp.f64 d0, d1    @ Compare d0 (t1) and d1 (t2)
    @ vmrs APSR_nzcv, fpscr  @ Move the result to APSR
    @ bge t1_more_than_t2  @ Branch if t1 >= t2

    @ @ t1 < t2
    @ @ r_y += r_vy * t1

    @ ldr r0, [sp, #4]   @ Load the address of r_y
    @ ldr r1, [sp, #32]  @ Load the address of r_vy
    @ ldr r2, [sp, #8]   @ Load the address of t1

    @ vldr.f64 d0, [r0]  @ Load r_y into d0
    @ vldr.f64 d1, [r1]  @ Load r_vy into d1
    @ vldr.f64 d2, [r2]  @ Load t1 into d2

    @ vmul.f64 d1, d1, d2  @ Multiply d1 (r_vy) by d2 (t1)
    @ vadd.f64 d0, d0, d1  @ Add the result to d0 (r_y)

    @ @ store back to r_y
    @ vstr.f64 d0, [sp, #4]  @ Store the result in r_y

    @ @ r_ix += r_ivx
    @ ldr r0, [sp, #20]  @ Load the address of r_ix
    @ ldr r1, [sp, #36]  @ Load the address of r_ivx

    @ vldr.f64 d0, [r0]  @ Load r_ix into d0
    @ vldr.f64 d1, [r1]  @ Load r_ivx into d1

    @ vadd.f64 d0, d0, d1  @ Add d1 (r_ivx) to d0 (r_ix)

    @ @ store back to r_ix
    @ vstr.f64 d0, [sp, #20]  @ Store the result in r_ix

    @ @ r_x = r_ix - (r_vx < 0) * r_ivx
    @ ldr r0, [sp, #12]  @ Load the address of r_x
    @ ldr r1, [sp, #20]  @ Load the address of r_ix
    @ ldr r2, [sp, #28]  @ Load the address of r_vx
    @ ldr r3, [sp, #36]  @ Load the address of r_ivx

    @ vldr.f64 d0, [r0]  @ Load r_x into d0
    @ vldr.f64 d1, [r1]  @ Load r_ix into d1
    @ vldr.f64 d2, [r2]  @ Load r_vx into d2
    @ vldr.f64 d3, [r3]  @ Load r_ivx into d3

    @ vcmp.f64 d2, #0.0  @ Compare d2 (r_vx) with 0.0
    @ vmrs APSR_nzcv, fpscr  @ Move the result to APSR
    @ @ itt ge
    @ @ vmov.i64 d2, #0  @ Set d2 (r_vx) to 0.0
    @ @ itt lt  @ If less than
    @ @ vmovlt.f64 d2, #1  @ Set d2 (r_vx) to 1.0
    @ vmov.f64 d2, #1  @ Set d2 (r_vx) to 1.0

    @ vmul.f64 d2, d2, d3  @ Multiply d2 (r_vx) by d3 (r_ivx)
    @ vsub.f64 d1, d1, d2  @ Subtract d2 (r_ivx) from d1 (r_ix)

    @ vstr.f64 d1, [sp, #12]  @ Store the result in r_x

    @ @ r_dist += t1
    @ ldr r0, [sp, #44]  @ Load the address of r_dist
    @ ldr r1, [sp, #8]   @ Load the address of t1

    @ vldr.f64 d0, [r0]  @ Load r_dist into d0
    @ vldr.f64 d1, [r1]  @ Load t1 into d1

    @ vadd.f64 d0, d0, d1  @ Add d1 (t1) to d0 (r_dist)

    @ vstr.f64 d0, [sp, #44]  @ Store the result in r_dist

    @ b end


t1_more_than_t2:
    @ t1 >= t2
    @ r_x += r_vx * t2

    ldr r0, [sp, #12]  @ Load the address of r_x
    ldr r1, [sp, #28]  @ Load the address of r_vx
    ldr r2, [sp, #16]  @ Load the address of t2

    vldr.f64 d0, [r0]  @ Load r_x into d0
    vldr.f64 d1, [r1]  @ Load r_vx into d1
    vldr.f64 d2, [r2]  @ Load t2 into d2

    vmul.f64 d1, d1, d2  @ Multiply d1 (r_vx) by d2 (t2)
    vadd.f64 d0, d0, d1  @ Add the result to d0 (r_x)

    vmov.f64 d0, #1.0  @ Set d0 (r_x) to 1.0

    @ store back to r_x
    vstr.f64 d0, [sp, #12]  @ Store the result in r_x

    @ r_iy += r_ivy
    ldr r0, [sp, #24]  @ Load the address of r_iy
    ldr r1, [sp, #40]  @ Load the address of r_ivy

    ldr r3, [r0]  @ Load r_iy into r3 (integer register)
    ldr r4, [r1]  @ Load r_ivy into r4 (integer register)

    add r3, r3, r4  @ Add r4 (r_ivy) to r3 (r_iy)

    @ store back to r_iy
    str r3, [sp, #24]  @ Store the result in r_iy

    @ r_y = r_iy - (r_vy < 0) * r_ivy
    ldr r0, [sp, #4]   @ Load the address of r_y
    ldr r1, [sp, #24]  @ Load the address of r_iy
    ldr r2, [sp, #32]  @ Load the address of r_vy
    ldr r3, [sp, #40]  @ Load the address of r_ivy

    @ ldr r4, [r1]

    @ ldr r0, =pattern
    @ bl printf

    @ vldr.f64 d0, [r0]  @ Load r_y into r4 (double register)
    @ vldr.i32 d1, [r1]  @ Load r_iy into r5 (double register)

    @ vldr.f64 d2, [r2]  @ Load r_vy into r5 (integer register)
    @ ldr r6, [r3]  @ Load r_ivy into r6 (integer register)
end:
    pop {pc}
    bx lr
