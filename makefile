# Makefile for the C program

# Compiler settings
CC = gcc
CFLAGS = -Wall -ansi

# Directories
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

EXEC = assembler

all: $(EXEC)

$(OBJ_DIR)/pre_processor.o: $(SRC_DIR)/pre-processor/pre_processor.c $(SRC_DIR)/pre-processor/support_functions.c $(SRC_DIR)/error-handling/errors.c
	$(CC) $(CFLAGS) $^ -o $@

$(OBJ_DIR)/first_pass.o: $(SRC_DIR)/first-pass/first_pass.c $(SRC_DIR)/first-pass/support-functions/encode.c $(SRC_DIR)/first-pass/support-functions/instruction.c $(SRC_DIR)/first-pass/support-functions/handle_data_directive.c $(SRC_DIR)/first-pass/support-functions/functions.c $(SRC_DIR)/first-pass/support-functions/line_formmating_functions.c
	$(CC) $(CFLAGS) -c $^ -o $@

# $(OBJ_DIR)/pre_processor.o: $(SRC_DIR)/pre-processor/pre_processor.c $(SRC_DIR)/pre-processor/support_functions.c
# 	$(CC) $(CFLAGS) $^ -o $@
# Clean target: remove object files and the binary directory
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Declare 'clean' as a phony target (not associated with a file)
.PHONY: clean
