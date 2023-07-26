#ifndef __INSTRUCTION_H
#define __INSTRUCTION_H

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

typedef struct encodedInstruction EncodedInstruction;
EncodedInstruction *encode_instruction(const char *instruction_line, const SymbolTable *table);
#endif