# Compiler and flags
CXX = g++
CXXFLAGS = -fdiagnostics-color=always -g -v -I/opt/X11/include -std=c++11 
LDFLAGS = -L/opt/X11/lib -lX11

# Directories
BASE_DIR = /Users/pee/Documents/project/RayCasting
SRC_DIR = $(BASE_DIR)
BUILD_DIR = $(BASE_DIR)
ASM_DIR = $(BASE_DIR)/asm

# Target executable
TARGET = main

# Source files
SRCS = $(wildcard $(SRC_DIR)/$(TARGET).cpp)
ASMSRCS = $(wildcard $(ASM_DIR)/*.s)

# Object files
OBJ = $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
ASM_OBJ = $(patsubst $(ASM_DIR)/%.s,$(BUILD_DIR)/%.o,$(ASMSRCS))

# Assembler and flags
AS = as -mfpu=neon-vfpv3
ASFLAGS =

# Build the target executable
$(BUILD_DIR)/$(TARGET): $(OBJ) $(ASM_OBJ)
	@echo "Linking $@"
	@$(CXX) -o $@ $(ASM_OBJ) $(OBJ) $(LDFLAGS) -lc

# Build rule for C++ source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "Compiling $<"
	@$(CXX) $(CXXFLAGS) -c -o $@ $<

# Build rule for assembly source files
$(BUILD_DIR)/%.o: $(ASM_DIR)/%.s
	@echo "Assembling $<"
	@$(AS) $(ASFLAGS) -o $@ $<

# Clean rule
clean:
	@echo "Cleaning up"
	@rm -f $(BUILD_DIR)/*.o $(BUILD_DIR)/$(TARGET)

# Phony target
.PHONY: clean
