CXX = g++
AS = as
CXXFLAGS = -fdiagnostics-color=always -g
LDFLAGS = -lX11
SRC_DIR = /home/pi/Desktop/RayCasting
BUILD_DIR = /home/pi/Desktop/RayCasting
TARGET = main
CPP_SRC = $(wildcard $(SRC_DIR)/$(TARGET).cpp)
AS_SRC = $(wildcard $(SRC_DIR)/*.s)
CPP_OBJ = $(CPP_SRC:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
AS_OBJ = $(AS_SRC:$(SRC_DIR)/%.s=$(BUILD_DIR)/%.o)
OBJ = $(CPP_OBJ) $(AS_OBJ)

all: $(BUILD_DIR)/$(TARGET)

$(BUILD_DIR)/$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.s
	$(AS) -o $@ $<

clean:
	rm -f $(BUILD_DIR)/*.o $(BUILD_DIR)/$(TARGET)

.PHONY: all clean