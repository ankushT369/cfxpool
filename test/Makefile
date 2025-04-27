# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -pthread

# Libraries to link
LIBS = -lgtest -lgtest_main

# Source files
C_SOURCES = ../fxpool.c ../fxerror.c ../fxlog.c ../fxsys.c
CPP_SOURCES = test.cpp

# Output executable
TARGET = test_program

# Build rule
all: $(TARGET)

$(TARGET): $(CPP_SOURCES) $(C_SOURCES)
	$(CXX) $(CXXFLAGS) $(CPP_SOURCES) $(C_SOURCES) $(LIBS) -o $(TARGET)

# Clean rule
clean:
	rm -f $(TARGET)

