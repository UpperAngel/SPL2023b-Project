# Compiler and compiler flags
CC = gcc
CFLAGS = -Wall -ansi 

# Directories
BUILD_DIR = build
BIN_DIR = bin

# Source directories
SRC_DIR = src
FIRST_PASS_DIR = $(SRC_DIR)/first-pass
PRE_PROCESSOR_DIR = $(SRC_DIR)/pre-processor
SECOND_PASS_DIR = $(SRC_DIR)/second-pass
SUPPORT_FUNCTIONS_DIR = $(FIRST_PASS_DIR)/support-functions

# Source files
ASSEMBLER_SRC = $(SRC_DIR)/assembler.c
CREATE_FILE_FUNC_SRC = $(SRC_DIR)/create_file_func.c
FIRST_PASS_SRC = $(FIRST_PASS_DIR)/first_pass.c $(SUPPORT_FUNCTIONS_DIR)/functions.c $(SUPPORT_FUNCTIONS_DIR)/handle_data_directive.c $(SUPPORT_FUNCTIONS_DIR)/line_foramting_functions.c
ERROR_HANDLING_SRC = $(SRC_DIR)/error-handling/errors.h
PRE_PROCESSOR_SRC = $(PRE_PROCESSOR_DIR)/pre_processor.c $(PRE_PROCESSOR_DIR)/support_functions.c
SECOND_PASS_SRC = $(SECOND_PASS_DIR)/second_pass.c

# Object files
ASSEMBLER_OBJ = $(BUILD_DIR)/assembler.o
CREATE_FILE_FUNC_OBJ = $(BUILD_DIR)/create_file_func.o
FIRST_PASS_OBJ = $(BUILD_DIR)/first_pass.o $(BUILD_DIR)/functions.o $(BUILD_DIR)/handle_data_directive.o $(BUILD_DIR)/line_foramting_functions.o
ERROR_HANDLING_OBJ = $(BUILD_DIR)/errors.o
PRE_PROCESSOR_OBJ =  $(BUILD_DIR)/pre_processor.o $(BUILD_DIR)/functions.o
SECOND_PASS_OBJ = $(BUILD_DIR)/second_pass.o

# Executable name
EXEC = $(BIN_DIR)/assembler

# Build the executable
all: $(EXEC)

# Link the object files to create the executable
$(EXEC): $(ASSEMBLER_OBJ) $(CREATE_FILE_FUNC_OBJ) $(FIRST_PASS_OBJ) $(ERROR_HANDLING_OBJ) $(PRE_PROCESSOR_OBJ) $(SECOND_PASS_OBJ) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^

# Compile the source files into object files
$(ASSEMBLER_OBJ): $(ASSEMBLER_SRC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $<

$(CREATE_FILE_FUNC_OBJ): $(CREATE_FILE_FUNC_SRC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $<

$(FIRST_PASS_OBJ): $(FIRST_PASS_SRC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $(filter %.c, $^)

$(ERROR_HANDLING_OBJ): $(ERROR_HANDLING_SRC) | $(BUILD_DIR)
	touch $@

$(PRE_PROCESSOR_OBJ): $(PRE_PROCESSOR_SRC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $<

$(SECOND_PASS_OBJ): $(SECOND_PASS_SRC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $<

# Create necessary directories
$(BUILD_DIR):
	mkdir -p $@

$(BIN_DIR):
	mkdir -p $@

# Clean up generated files
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

.PHONY: all clean
