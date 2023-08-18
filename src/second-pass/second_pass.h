#ifndef __SECOND_PASS_H
#define __SECOND_PASS_H
#include "error.h"
#include "../first-pass/first-pass-headers/first_pass_headers.h"

void second_pass(const char *file_name, Symbol *symbol_table, struct InstructionStructure instruction_array[], struct DataStructure data_array[], struct SymbolNameAndIndex *symbol_name_and_index, int *error_found);

#endif