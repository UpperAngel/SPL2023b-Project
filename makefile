# Compiler settings
CC = gcc
CFLAGS = -Wall -ansi

# Directories
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

EXEC = $(BIN_DIR)/assembler

all: $(EXEC)

$(OBJ_DIR)/errors.o: $(SRC_DIR)/error-handling/errors.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/error-handling/errors.c -o $@

$(OBJ_DIR)/support_functions.o: $(SRC_DIR)/pre-processor/support_functions.c $(SRC_DIR)/pre-processor/pre-processor-headers/support_functions.h
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/pre-processor/support_functions.c -o $@

$(OBJ_DIR)/pre_processor.o: $(SRC_DIR)/pre-processor/pre_processor.c $(OBJ_DIR)/support_functions.o $(OBJ_DIR)/errors.o
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/pre-processor/pre_processor.c -o $@

$(OBJ_DIR)/globals.o: $(SRC_DIR)/globals/globals.c $(SRC_DIR)/globals/globals.h
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/globals/globals.c -o $@

$(OBJ_DIR)/encode.o: $(SRC_DIR)/first-pass/support-functions/encode.c $(SRC_DIR)/first-pass/first-pass-headers/encode.h $(OBJ_DIR)/globals.o
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/first-pass/support-functions/encode.c -o $@

$(OBJ_DIR)/instruction.o: $(SRC_DIR)/first-pass/support-functions/instruction.c $(SRC_DIR)/first-pass/first-pass-headers/instruction.h
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/first-pass/support-functions/instruction.c  -o $@

$(OBJ_DIR)/functions.o: $(SRC_DIR)/first-pass/support-functions/functions.c $(OBJ_DIR)/encode.o $(OBJ_DIR)/globals.o
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/first-pass/support-functions/functions.c -o $@

$(OBJ_DIR)/first_pass.o: $(SRC_DIR)/first-pass/first_pass.c $(OBJ_DIR)/functions.o $(OBJ_DIR)/encode.o $(OBJ_DIR)/globals.o
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/first-pass/first_pass.c -o $@

$(OBJ_DIR)/second_pass.o: $(SRC_DIR)/second-pass/second_pass.c $(OBJ_DIR)/encode.o $(OBJ_DIR)/errors.o $(OBJ_DIR)/functions.o $(OBJ_DIR)/instruction.o $(OBJ_DIR)/globals.o
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/second-pass/second_pass.c  -o $@

$(EXEC): $(SRC_DIR)/assembler.c $(OBJ_DIR)/second_pass.o $(OBJ_DIR)/pre_processor.o $(OBJ_DIR)/first_pass.o $(OBJ_DIR)/support_functions.o $(OBJ_DIR)/errors.o $(OBJ_DIR)/encode.o $(OBJ_DIR)/instruction.o $(OBJ_DIR)/functions.o  $(OBJ_DIR)/globals.o
	mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(SRC_DIR)/assembler.c $(OBJ_DIR)/second_pass.o $(OBJ_DIR)/pre_processor.o $(OBJ_DIR)/first_pass.o $(OBJ_DIR)/support_functions.o $(OBJ_DIR)/errors.o $(OBJ_DIR)/encode.o $(OBJ_DIR)/instruction.o $(OBJ_DIR)/functions.o  $(OBJ_DIR)/globals.o -o $(EXEC) -g


# Clean target: remove object files and the binary directory
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Declare 'clean' as a phony target (not associated with a file)
.PHONY: clean
