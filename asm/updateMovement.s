.global _modul0
.global _add0
.global _multiply1
.global _absolute0
.global _testlessthan

.align 4

_modul0:
    // Input: x0 = dividend (a)
    //        x1 = divisor  (b)
    // Output: x0 = result (a % b)

    // Check if divisor is zero (division by zero is undefined)
    cbz    w1, handle_div_by_zero


    // r = a / b
    sdiv   w2, w0, w1
    
    // r = r * b
    mul    w2, w2, w1
    
    // remainder = a - r
    sub    w0, w0, w2
    
    ret

handle_div_by_zero:
    // Handle division by zero as you see fit.
    // For simplicity, we'll just return 0.
    mov    w0, #0
    ret


_add0:
    // Input:
    // x: x0 register
    // y: x1 register

    // Perform addition
    add w0, w0, w1

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
    
absolute:
    neg x0, x0
    ret

/*

_absolute0:
    // Input:
    // x: x0 register

    // Check if x is negative
    cmp x0, #0
    bge .end

    // If x is negative, flip the sign
    neg x0, x0

.end:
    // x0 now contains the absolute value of x
    ret

_testlessthan:
    cmp x0, #0         ; compare r0 to 0
    blt .absolute       ; if r0 < 0, branch to absolute
    ret              ; else, return to caller

*/

