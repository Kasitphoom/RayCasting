CXX = g++
CXXFLAGS = -fdiagnostics-color=always -g
LDFLAGS = -lX11
SRC_DIR = /home/pi/Desktop/RayCasting
BUILD_DIR = /home/pi/Desktop/RayCasting
TARGET = main
SRC = $(wildcard $(SRC_DIR)/$(TARGET).cpp)
OBJ = $(SRC:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

$(BUILD_DIR)/$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(BUILD_DIR)/*.o $(BUILD_DIR)/$(TARGET)

.PHONY: clean