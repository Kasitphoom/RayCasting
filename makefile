CXX = g++
CXXFLAGS = -fdiagnostics-color=always -g
LDFLAGS = -lX11
SRC_DIR = /home/pi/Proj/RayCasting
ASM_SRC_DIR = /home/pi/Proj/RayCasting/asm
BUILD_DIR = /home/pi/Proj/RayCasting
TARGET = main
SRC = $(wildcard $(SRC_DIR)/$(TARGET).cpp)
ASM_SRC = $(wildcard $(ASM_SRC_DIR)/*.s)
OBJ = $(SRC:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
ASM_OBJ = $(ASM_SRC:$(ASM_SRC_DIR)/%.s=$(BUILD_DIR)/%.o)

$(BUILD_DIR)/%.o: $(ASM_SRC_DIR)/%.s
	as -march=armv7-a -mfpu=neon-vfpv3 -o $@ $<

$(BUILD_DIR)/$(TARGET): $(OBJ) $(ASM_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(BUILD_DIR)/*.o $(BUILD_DIR)/$(TARGET)

.PHONY: clean
