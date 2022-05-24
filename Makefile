
# Makefile for the maze game.

CC=gcc

# -Wall enables all the warning about constructions.
# -O2 is for level 2 optimization which reduce the execution time.
CFLAGS=-Wall -O2 -c 

# `src` is the directory where all the .c files are.
SRC=src

# `include` is the directory where all the .h files are
INCLUDE=include

# `bin` is the directory where the final maze executable will be.
BIN=bin

# This is the executable file name.
EXE_PROD=server.prod
EXE_DEV=server.dev

OBJ=obj

all: dev

# Using wild card to create object file at once 
# Using -I to add the header files in `include` directory
# as standard lib.
%.o: $(SRC)/%.c
	$(CC) $(STD) $(CFLAGS) -I$(INCLUDE) $^ -o $(OBJ)/$@

prod: main.o error.o 
	$(CC) $(OBJ)/main.o $(OBJ)/error.o -o $(BIN)/$(EXE_PROD)

dev: main.o error.o 
	$(CC) $(OBJ)/main.o $(OBJ)/error.o -o $(BIN)/$(EXE_DEV)

tests:
	echo Running Tests.

# Cleans only the .o files for a clean build 
clean_obj: 
	rm -fr *.o

# Clean all the binary executable and *.o files
clean:
	rm -fr *.o $(BIN)/* $(OBJ)/*
