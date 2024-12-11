# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -I../game -Isdl/include
LDFLAGS = -Lsdl/lib -lmingw32 -lSDL2main -lSDL2

# Directories
SRC_DIR = ../game
TEST_DIR = .

# Find all the C files in src and test
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
TEST_FILES = $(wildcard $(TEST_DIR)/*.c)

# Output binary name
OUTPUT = test

# Build target
$(OUTPUT): $(SRC_FILES) $(TEST_FILES)
	$(CC) $(CFLAGS) $(SRC_FILES) $(TEST_FILES) $(LDFLAGS) -o $(OUTPUT)

# Clean target
clean:
	rm -f $(OUTPUT)
