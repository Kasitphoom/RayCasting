.global _calculate_index

.text
.align 2

_calculate_index:
    // Input arguments:
    // x:    x0 (int)
    // y:    x1 (int)
    // res_X: x2 (int)

    mov x3, x0

    // Calculate index
    mul x0, x1, x2  // Multiply y by res_X
    add x0, x0, x3  // Add the original value of x to the result

    ret
