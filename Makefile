# List of source files
SRCS = fxerror.c fxlog.c fxpool.c fxsys.c

# Object files (placed in build/)
OBJS = $(SRCS:.c=.o)
BUILD_OBJS = $(addprefix build/, $(OBJS))

# Include and output directories
INCLUDE_DIR = .
BUILD_DIR = build

# Compiler and flags
CC ?= gcc
CFLAGS = -fPIC -I$(INCLUDE_DIR)

# Targets
all: $(BUILD_DIR)/libcfx.so $(BUILD_DIR)/libcfx.a

# Create build directory if not exists
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Compile .c to .o into build/
$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create shared library
$(BUILD_DIR)/libcfx.so: $(BUILD_OBJS)
	$(CC) -shared -o $@ $^

# Create static library
$(BUILD_DIR)/libcfx.a: $(BUILD_OBJS)
	ar rcs $@ $^

# Clean up
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean

