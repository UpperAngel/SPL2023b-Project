#include "../first-pass/first-pass-headers/symbol_table.h"

void second_pass(SymbolTable *symbol_table, instr,struct symbolNamePlusIndex *symbolNamePlusIndex) {
    int i;
    char *curr_symbol_name;

    if (get_symbol_by_name(symbol_table, curr_symbol_name) != NULL) {
        
    }
}

/*
gets name of label, index.

lookup name -> go to index -> encode

*/