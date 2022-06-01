
# Makefile for the maze game.

CC=gcc

# -Wall enables all the warning about constructions.
# -O2 is for level 2 optimization which reduce the execution time.
CFLAGS=-Wall  -c 

# `src` is the directory where all the .c files are.
SRC=src

# `include` is the directory where all the .h files are
INCLUDE=lib

# `bin` is the directory where the final maze executable will be.
BIN=bin

# This is the executable file name.
EXE=server.prod

OBJ=obj

all: build

# Using wild card to create object file at once 
# Using -I to add the header files in `include` directory
# as standard lib.
%.o: $(SRC)/%.c
	$(CC) $(STD) $(CFLAGS) -I$(INCLUDE) $^ -o $(OBJ)/$@

build: main.o error.o get.o server.o serve.o proxy.o file.o
	$(CC) $(OBJ)/main.o $(OBJ)/error.o $(OBJ)/get.o $(OBJ)/server.o $(OBJ)/serve.o $(OBJ)/proxy.o $(OBJ)/file.o -o $(BIN)/$(EXE)

# Cleans only the .o files for a clean build 
clean_obj: 
	rm -fr *.o

# Clean all the binary executable and *.o files
clean:
	rm -fr *.o $(BIN)/* $(OBJ)/*
