.data

.text
.global _compute_r_angle
.align 4

_compute_r_angle:
    ; Input parameters:
    ;   w0: xs (int)
    ;   w1: res_X (int)
    ;   w2: fov (int)
    ;   s0: player.ang_h (float)

    scvtf s1, w0             ; Convert xs (integer) to a single-precision float and store it in s1
    scvtf s2, w1             ; Convert res_X (integer) to a single-precision float and store it in s2
    scvtf s3, w2             ; Convert fov (integer) to a single-precision float and store it in s3

    fdiv s5, s2, s3          ; Divide s2 (res_X) by s3 (fov) and store it in s5 (as a single-precision float)
    fsub s4, s1, s5          ; Calculate (xs - res_X / fov) and store the result in s4 (as a single-precision float)

    ; Load the constant 3600.0 into a general-purpose register (x6)
    ldr x6, =3600

    ; Transfer the value from x6 (general-purpose) to s6 (floating-point)
    scvtf s6, x6

    fmul s4, s4, s3          ; Multiply the result in s4 by s3 (fov) and store the result in s4 (as a single-precision float)
    fdiv s4, s4, s2          ; Divide the result in s4 by s2 (res_X) and store the result in s4 (as a single-precision float)

    fadd s4, s4, s0          ; Add player.ang_h to the result and store the final result in s4 (as a single-precision float)

    fadd s4, s4, s6          ; Multiply the result in s4 by s6 (3600.0) and store the result in s4 (as a single-precision float)
    fcvtzs w0, s4            ; Convert the result in s4 (single-precision float) to an integer and store it in w0 (r_angle)

    ; Check for integer overflow
    ldr x7, =2147483647      ; Load the maximum 32-bit signed integer to x7
    ldr x8, =-2147483648     ; Load the minimum 32-bit signed integer to x8

    scvtf s7, x7             ; Convert x7 (maximum) to a single-precision float and store it in s7
    scvtf s8, x8             ; Convert x8 (minimum) to a single-precision float and store it in s8

    fcmp s4, s7              ; Compare with the maximum 32-bit signed integer
    bhi overflow             ; Branch if greater than or equal to max integer
    fcmp s4, s8              ; Compare with the minimum 32-bit signed integer
    blt overflow             ; Branch if less than min integer

    ret

overflow:
    ; Handle overflow here (if necessary)
    ; You can set w0 to a maximum or minimum value
    mov w0, #2147483647      ; Set w0 to the maximum 32-bit signed integer
    ret
