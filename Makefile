# Compiler to use
CC = gcc

# Compiler flags, -I adds the include directory to the search path for header files
CFLAGS = -Wall -Iinclude

# Final output binary name
OUTPUT = main.exe

# Directories
SRC_DIR = source
OBJ_DIR = obj
INC_DIR = include

# Source files (excluding main.c)
SOURCES = $(wildcard $(SRC_DIR)/*.c)

# Object files (derived from SOURCES, placed in OBJ_DIR)
OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SOURCES))

# Object file for main.c (assuming main.c is in the root)
MAIN_OBJ = $(OBJ_DIR)/main.o

# Default target
all: $(OUTPUT)

# Link object files into a binary
$(OUTPUT): $(MAIN_OBJ) $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@

# Compile main.c into its object file
$(MAIN_OBJ): main.c
	$(CC) $(CFLAGS) -c $< -o $@

# Compile .c files in SRC_DIR into .o files in OBJ_DIR
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up
clean:
	rm -rf $(OBJ_DIR) $(OUTPUT)

# Prevent make from doing something with a file named all or clean
.PHONY: all clean
