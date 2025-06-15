# Makefile for Rocket Game (Windows)

# Compiler and flags
CXX = g++
CXXFLAGS = -Iheader -Wall
LDFLAGS = -lwinmm

# Source and object files
SRCDIR = src
SRC = $(wildcard $(SRCDIR)/*.cpp)
OBJ = $(SRC:.cpp=.o)
MAIN = main.cpp
TARGET = rocket_game

# Default target
all: $(TARGET)

# Link object files into executable
$(TARGET): $(OBJ) $(MAIN)
	$(CXX) $(CXXFLAGS) $(OBJ) $(MAIN) -o $(TARGET) $(LDFLAGS)

# Clean build artifacts
clean:
	del /Q $(SRCDIR)\*.o $(TARGET).exe 2>nul || true

# Run the program
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
