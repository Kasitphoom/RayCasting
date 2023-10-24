.global hmap_calc
hmap_calc:
    @ r0 = addr hmap[xs], r1 = res_Y, d0 = r_dist, d1 = fisheye[xs]
    @ hmap[xs] = (int)(res_Y / 2 / r_dist / fisheye[xs])

    vmov s0, r1
    vcvt.f64.s32 d2, s0

    vmov.f64 d3, #2
    vdiv.f64 d2, d2, d3
    
    vdiv.f64 d2, d2, d0
    vdiv.f64 d2, d2, d1

    vcvt.s32.f64 s0, d2
    vmov r1, s0

    str r1, [r0]
    mov r0, r1

    bx lr
