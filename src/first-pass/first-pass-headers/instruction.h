#ifndef __INSTRUCTION_H
#define __INSTRUCTION_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symbol_table.h"
#include "instruction_helper.h"
enum addressingType {
    ERROR = 0,
    DIRECT = 3,
    IMMEDIATE = 5,
    REGISTER = 1,
};
typedef struct encodedInstruction {
    unsigned int encoding_type : 2;
    unsigned int target_addressing : 3;
    unsigned int opcode : 4;
    unsigned int source_addressing : 3;
} EncodedInstruction;

int encode_instruction(EncodedInstruction *encoded, const char *instruction_line, const SymbolTable *table);
char *get_content(const char *line);
int is_directive(const char *line);
int is_extern(const char *directive);
int is_entry(const char *directive);
int is_data(const char *directive);
int is_valid_line_length(const char *line);
int is_string(const char *line);
int is_valid_string(const char *line);

#endif