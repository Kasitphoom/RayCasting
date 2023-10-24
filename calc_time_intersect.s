@ d0 = r_x, d1 = r_vx

.global calc_t
calc_t:
    
    @ Access the double-precision arguments in the appropriate registers
    vmov.f64 d2, d1
    vmov.f64 d1, d0

    @ If r_vx is more than 0
    vcmp.f64 d2, #0
    vmrs APSR_nzcv, fpscr
    ble no_condition  @ Branch if less than or equal to zero
    vmov.f64 d0, #1 @ Load the IEEE 754 representation of 1.0 (0x3FF0000000000000) into r0
    vmov.f64 d4, d0
    b done

no_condition:
    vmov.i64 d0, #0  @ Load the IEEE 754 representation of 0.0 (0x0000000000000000) into r0
    vmov.f64 d4, d0

done:
    @ r_ix - r_x + condition
    vmov.f32 s0, r0
    vcvt.f64.s32 d0, s0
    vsub.f64 d0, d0, d1
    vadd.f64 d0, d0, d4

    @ Divide by r_vx
    vdiv.f64 d0, d0, d2
    bx lr
