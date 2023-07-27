#ifndef __INSTRUCTION_H
#define __INSTRUCTION_H

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

typedef struct encodedInstruction EncodedInstruction;
typedef struct instruction Instruction;

EncodedInstruction *encode_instruction(const char *instruction_line, const SymbolTable *table);
int calc_l(const char *line, SymbolTable *table);
char *get_content(const char *line);
int is_directive(const char *line);
#endif