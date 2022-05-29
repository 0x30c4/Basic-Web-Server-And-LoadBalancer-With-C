
# Makefile for the maze game.

CC=gcc

# -Wall enables all the warning about constructions.
# -O2 is for level 2 optimization which reduce the execution time.
CFLAGS=-Wall -O2 -c 

# `src` is the directory where all the .c files are.
SRC=../src

# `include` is the directory where all the .h files are
INCLUDE=../lib

# `bin` is the directory where the final maze executable will be.
BIN=../bin
OBJ=../obj
TEST_SOURCE=test_source

all: dev

# Using wild card to create object file at once 
# Using -I to add the header files in `include` directory
# as standard lib.
%.o: $(SRC)/%.c
	$(CC) $(STD) $(CFLAGS) -I$(INCLUDE) $^ -o $(OBJ)/$@

%.to: $(TEST_SOURCE)/%.c
	$(CC) $(STD) $(CFLAGS) -I$(INCLUDE) $^ -o $(OBJ)/$@

test_get: get.to get.o file.o serve.o
	$(CC) $(OBJ)/get.to $(OBJ)/get.o $(OBJ)/file.o $(OBJ)/serve.o -o $(BIN)/get.dev


test_server: server.to get.o file.o serve.o server.o
	$(CC) $(OBJ)/server.to $(OBJ)/get.o $(OBJ)/file.o $(OBJ)/serve.o $(OBJ)/server.o -o $(BIN)/server.t.dev

# Cleans only the .o files for a clean build 
clean_obj: 
	rm -fr *.o

# Clean all the binary executable and *.o files
clean:
	rm -fr *.o $(BIN)/* $(OBJ)/*
