.global _calculate_index

.text
.align 2


_calculate_index:
    // Input arguments:
    // x:    x0
    // y:    x1
    // res_X: x2
    // index: x3 (pointer to int)

    // Dereference the index pointer and store the result in x4
    ldr x4, [x3]

    // Calculate index
    mul x4, x1, x2  // Multiply y by res_X
    add x4, x4, x0  // Add x to the result

    // Store the calculated index back into memory
    str x4, [x3]
    ret
