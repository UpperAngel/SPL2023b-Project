#ifndef __FIRST_PASS_H
#define __FIRST_PASS_H

#include <stdlib.h>
#include "first_pass_headers.h"
#include "encode.h"
#include "instruction.h"

void first_pass(FILE *am_file, struct InstructionStructure *instructions_array, struct DataStructure *data_array, Symbol **symbol_head, struct SymbolNameAndIndex **second_pass_list, int *ic, int *dc, int *error_found);


#endif