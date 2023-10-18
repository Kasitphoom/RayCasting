.global _modul0
.global _add0
.global _multiply1

.align 4

_modul0:
    // Input:
    // x: x0 register
    // y: x1 register

    // Check for y = 0, which is undefined behavior
    cmp x1, #0
    beq .zero_division_error

    // Compute the remainder using division and multiplication
    sdiv x3, x0, x1   // x3 = x / y
    mul x4, x3, x1   // x4 = x3 * y
    sub x0, x0, x4   // x0 = x - (x3 * y)

    // x0 now contains the remainder
    ret

_add0:
    // Input:
    // x: x0 register
    // y: x1 register

    // Perform addition
    add x0, x0, x1

    // x0 now contains the result of x + y
    ret

_multiply1:
    // Input:
    // x: x0 register
    // y: x1 register

    // Perform multiplication
    fmul d0, d0, d1

    // x0 now contains the result of x * y
    ret

.zero_division_error:
    // Handle division by zero error here (e.g., return an error code)
    // You can customize this part as needed

    // Example: Return -1 to indicate an error
    mov x0, #-1
    ret


