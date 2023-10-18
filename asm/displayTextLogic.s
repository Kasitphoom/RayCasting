.global _calculate_index
.global _calculate_x_coordinate
.global _calculate_y_coordinate
.global _calculate_rect_height

.text
.align 4

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

_calculate_x_coordinate:
    // Input arguments:
    // x:        x0 (int)
    // CHAR_WIDTH: x1 (int)

    // Calculate the x coordinate
    mul x0, x0, x1  // Calculate x = x * CHAR_WIDTH

    ret

_calculate_y_coordinate:
    // Input arguments:
    // y:        x0 (int)
    // CHAR_HEIGHT: x1 (int)
    mov x3 , 2

    // Calculate the y coordinate
    mul x0, x0, x1  // Calculate y = y * CHAR_HEIGHT * 2
    mul x0, x0, x3 // 

    ret

_calculate_rect_height:
    // Input arguments:
    // CHAR_HEIGHT: x0 (int)

    // Calculate the rectangle height
    lsl x0, x0, #1  // Multiply CHAR_HEIGHT by 2

    ret
