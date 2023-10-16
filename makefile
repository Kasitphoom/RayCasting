# Compiler and flags
CXX = g++
CXXFLAGS = -fdiagnostics-color=always -g -v -I/opt/X11/include -std=c++11
LDFLAGS = -L/opt/X11/lib -lX11

# Directories
SRC_DIR = /Users/pee/Documents/project/RayCasting
BUILD_DIR = /Users/pee/Documents/project/RayCasting

# Target executable
TARGET = main

# Source files
SRCS = $(wildcard $(SRC_DIR)/$(TARGET).cpp)
ASMSRCS = displayTextLogic.asm

# Object files
OBJ = $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
ASM_OBJ = $(ASMSRCS:.asm=.o)

# Assembler and flags
AS = as
ASFLAGS = 

# Build the target executable
$(BUILD_DIR)/$(TARGET): $(OBJ) $(ASM_OBJ)
	$(CXX) -o $@ $(ASM_OBJ) $(OBJ) $(LDFLAGS)

# Build rule for C++ source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Build rule for assembly source files
$(ASM_OBJ): $(ASMSRCS)
	$(AS) $(ASFLAGS) -o $@ $<

# Clean rule
clean:
	rm -f $(BUILD_DIR)/*.o $(BUILD_DIR)/$(TARGET)

# Phony target
.PHONY: clean
