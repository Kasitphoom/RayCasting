/* .global _test

.extern _sintab
.extern _printf

_test:
    // Load the address of sintab into x0
    ldr x0, =_sintab

    // Load the first value of sintab into d0 (since it's a double)
    ldr d0, [x0]

    // Set up arguments for printf
    // In ARM64, the first parameter is passed in x0, the second in x1, and so on.
    // Floating point parameters are passed in v0, v1, etc.
    ldr x0, =format_string  // Load the address of format_string into x0
    // Call _printf
    bl _printf

    ret

format_string:
    .asciz "Value from sintab: %lf\n"

*/
