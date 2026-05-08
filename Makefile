.PHONY: run clean

# Command: g++ -O2 -march=native -std=c++17 main.cpp -o benchmark

CXX = g++
CXXFLAGS = -O2 -march=native -std=c++17

OUT_DIR = bin
TARGET = $(OUT_DIR)/benchmark
SRCS = main.cpp

# Default target: build and run
run: 
	mkdir -p $(OUT_DIR)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
