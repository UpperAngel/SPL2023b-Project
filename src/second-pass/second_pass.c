#include "../first-pass/first-pass-headers/symbol_table.h"
#include "data-and-instructions.h"
#include "symbolNameAndIndex.h"
/*
gets name of label, index.

lookup name -> go to index -> encode

*/
void second_pass(SymbolTable *symbol_table, struct InstructionStructure **instruction_array, struct DataStructure *data_array, struct SymbolNameAndIndex *symbol_name_and_index) {
    struct SymbolNameAndIndex *curr_missing = symbol_name_and_index;
    while (curr_missing != NULL) {
        Symbol *symbol = get_symbol_by_name(symbol_table, curr_missing->name);

        if (symbol != NULL) {
            if (symbol->type == EXTERN) {
                instruction_array[curr_missing->IC_index]->encoding_type = 0x3;
            } else {
                instruction_array[curr_missing->IC_index]->encoding_type = 0x1;
            }
            in
        }
        curr_missing = curr_missing->next;
    }
}
