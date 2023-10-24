
.global update_r_y
.global update_r_ix
.global update_r_x
.global update_r_dist
.global compare_t1_t2
update_r_y:
    @ r0 = addr r_y, d0 = r_vy, d1 = t1
    @ r_y += r_vy * t1
    mov r3, r0

    vmul.f64 d0, d0, d1
    vldr.f64 d1, [r3]
    vadd.f64 d0, d0, d1
    vstr.f64 d0, [r3]
    bx lr

update_r_ix:
    @ r0 = addr r_ix, r1 = r_ivx
    @ r_ix += r_ivx
    mov r3, r0

    ldr r3, [r3]
    add r3, r3, r1
    str r3, [r0]
    bx lr

update_r_x:
    @ r0 = r_ix, d0 = r_vx, r1 = r_ivx
    @ return r_ix - (r_vx < 0) * r_ivx
    vcmp.f64 d0, #0.0
    vmrs apsr_nzcv, fpscr
    ite lt
    movlt r3, #1
    movge r3, #0
    
    mul r1, r1, r3
    sub r0, r0, r1

    vmov.f32 s0, r0
    vcvt.f64.s32 d0, s0

    bx lr

update_r_dist:
    @ r0 = addr r_dist, d0 = t1
    @ r_dist += t1

    mov r3, r0
    vldr.f64 d1, [r3]
    vadd.f64 d0, d0, d1
    vstr.f64 d0, [r3]
    bx lr

compare_t1_t2:
    @ d0 = t1, d1 = t2
    @ return t1 < t2
    vcmp.f64 d0, d1
    vmrs apsr_nzcv, fpscr
    ite lt
    movlt r0, #1
    movge r0, #0
    bx lr
