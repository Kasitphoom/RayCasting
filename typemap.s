.global typemap_func
.extern modulo
typemap_func:
    push {lr}

    mov r1, #256
    bl modulo

    sub r0, r0, #1

    pop {pc}
    bx lr

.global check_multiply
.extern fabs
.extern compare_t1_t2
check_multiply:
    @ r0 = boolean, r1 = constant, d0 = fabs(r_y - (int)(r_y)), d1 = fabs(r_x - (int)(r_x))
    @ (t1 < t2) ? 32 * fabs(r_y - (int)(r_y)) : 32 * fabs(r_x - (int)(r_x))
    push {lr}
    cmp r0, #1

    bne morethan
    
    @ t1 < t2
    vmov s0, r1
    vcvt.f64.s32 d2, s0
    vmul.f64 d0, d0, d2
    
    b end
morethan:
    @ t1 >= t2
    vmov s0, r1
    vcvt.f64.s32 d2, s0
    vmul.f64 d0, d1, d2
    b end

end:
    pop {pc}

.global update_lmap
update_lmap:
    @ r0 = addr lmap, d0 = constant, r1 = res_Y, r2 = hmap[xs]
    @ lmap[xs] *= 5.0 / res_Y * hmap[xs];
    push {lr}
    vmov d3, d0
    vmov s1, r1
    vcvt.f64.s32 d1, s1
    
    vmov s2, r2
    vcvt.f64.s32 d2, s2

    vdiv.f64 d3, d3, d1
    vmul.f64 d3, d3, d2
    vmov d0, d3
    
    vstr d0, [r0]
    
    pop {pc}
