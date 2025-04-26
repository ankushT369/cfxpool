# Variables
CC = gcc
CFLAGS = -Wall -Werror -I.      # Compiler flags: show warnings, use current directory for headers
SRC = main.c fxerror.c fxlog.c fxpool.c fxsys.c  # All your source files
OBJ = $(SRC:%.c=build/%.o)      # Object files will be in build/ folder
TARGET = build/exec             # Final executable in build/ folder

# Default target
all: $(TARGET)

# How to build the final executable
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

# How to compile .c to .o files into build/
build/%.o: %.c
	@mkdir -p build              # Make sure build folder exists
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -rf build

# Phony targets
.PHONY: all clean

