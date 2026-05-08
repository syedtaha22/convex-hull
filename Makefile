.PHONY: run clean

CXX = g++
CXXFLAGS = -O2 -march=native -std=c++17

TARGET = benchmark
SRCS = main.cpp

# Default target: build and run
run: $(TARGET)
	./$(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)

clean:
	rm -f $(TARGET)
