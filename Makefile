LD_FLAGS=-lSDL3
SRC_DIR=src
OBJ_DIR=.
CC=gcc
CFLAGS=-Wall -g

all: main

main: $(OBJ_DIR)/main.o $(OBJ_DIR)/algorithms.o
	$(CC) $^ $(LD_FLAGS) -o $@

$(OBJ_DIR)/main.o: $(SRC_DIR)/main.c $(SRC_DIR)/config.h
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/algorithms.o: $(SRC_DIR)/algorithms.c $(SRC_DIR)/algorithms.h
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJ_DIR)/*.o main
