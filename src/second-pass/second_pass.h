#ifndef __SECOND_PASS_H
#define __SECOND_PASS_H
#include "../globals/globals.h"
#include "../error-handling/errors.h"
#include "../first-pass/first-pass-headers/first_pass_headers.h"
#include "../first-pass/first-pass-headers/encode.h"
#include "../globals/globals.h"

void binaryToBase64(uint64_t binary, char *base64);
void create_files(const char *file_name, Symbol *curr_symbol, struct SymbolNameAndIndex *symbol_name_and_index, struct InstructionStructure instruction_array[], struct DataStructure data_array[], int IC, int DC);
void second_pass(const char *file_name, Symbol *symbol_table, struct InstructionStructure instruction_array[], struct DataStructure data_array[], struct SymbolNameAndIndex *symbol_name_and_index, int ic, int dc, int *error_found);

#endif