import os
#skull = os.path.join("Users","pee", "Documents", "generatePic", "skull.jpg")
from PIL import Image

def image_to_2d_array(image_path):
    # Open the image using PIL
    img = Image.open(image_path)
    
    # Ensure the image has the correct dimensions
    if img.size != (300, 60):
        raise ValueError("Image dimensions must be 300x100")
    
    # Convert image to grayscale (for ensuring we deal only with B&W)
    img = img.convert('L')
    
    # Create the 2D array
    array_2d = [[0 for _ in range(60)] for _ in range(300)]
    
    for x in range(300):
        for y in range(60):
            pixel_value = img.getpixel((x, y))
            
            # Check if pixel is black or close to black (threshold can be adjusted)
            if pixel_value < 128:  
                array_2d[x][y] = 1
    
    return array_2d

# Test the function
image_path = "/Users/pee/Documents/generatePic/skull.png"  # Replace with your image path
result = image_to_2d_array(image_path)

# Print the result (or process it as you need)
# for row in result:
#     print(row)


def rotate_90_degrees(matrix):
    # Step 1: Create a new 2D array with reversed dimensions
    rotated = [[0 for _ in range(300)] for _ in range(60)]
    
    # Step 2: Transpose the matrix
    for i in range(300):
        for j in range(60):
            rotated[j][i] = matrix[i][j]
    
    # Step 3: Reverse the columns
    for i in range(60):
        rotated[i] = rotated[i][::-1]
        
    return rotated

# Now, use the function on your 2D array:
array_rotated = rotate_90_degrees(result)

# Print the rotated array:
for row in array_rotated:
    print(row)


# /opt/homebrew/bin/python3 /Users/pee/Documents/generatePic/main.py