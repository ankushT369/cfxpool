# Paths
BUILD_DIR = ../build
INCLUDE_DIR = ..
SRC = main.c
OUT = program

# Compiler and flags
CC = gcc
CFLAGS = -I$(INCLUDE_DIR)
LDFLAGS = -L$(BUILD_DIR) -lcfx

# Default target
all: $(OUT)

# Build the example program
$(OUT): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(OUT) $(LDFLAGS)

# Run the example with correct library path
run: $(OUT)
	LD_LIBRARY_PATH=$(BUILD_DIR) ./$(OUT)

# Clean the executable
clean:
	rm -f $(OUT)

.PHONY: all run clean

