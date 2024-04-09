# Compiler
CC=gcc
# Compiler Flags
CFLAGS=-Iinclude

# Find all .c files in the src directory
SRCS=$(wildcard src/*.c)

# Object files to build
OBJS=$(SRCS:.c=.o) main.o

# Executable name
EXEC=main.exe

# Default target
all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean
clean:
	del /Q src\*.o *.o $(EXEC)

.PHONY: all clean
