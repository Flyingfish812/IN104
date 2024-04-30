# Compiler
CC=gcc
# Compiler Flags
CFLAGS=-Iinclude

# Find all .c files in the src directory
SRCS=$(wildcard src/*.c)

# Object files to build
OBJS=$(SRCS:.c=.o) main.o

# Executable name
EXEC=main

# Default target
all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean
clean:
<<<<<<< HEAD
	rm -rf src/*.o *.o $(EXEC)
=======
	rm -rf src\*.o *.o $(EXEC)
>>>>>>> 56d79b6a46d45548f191233a9f0c4367e038c46d

.PHONY: all clean
