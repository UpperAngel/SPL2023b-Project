#include <stdio.h>

#include "src/first-pass/first-pass-headers/symbol_table.h"
#include "src/pre-processor/pre-processor header files/helper_fucntions.h"

int main(int argc, char const *argv[]) {
    SymbolTable *symbol_table = init_table(1);
    Symbol *curr_symbol;
    
    add_symbol(symbol_table, "A", 10, ENTRY);
    add_symbol(symbol_table, "B", 1, ENTRY);
    add_symbol(symbol_table, "C", 23, ENTRY);
    add_symbol(symbol_table, "D", 41, ENTRY);
    
    int i;
    FILE *ent_file = create_file("help", ".ent");

    for (i = 0; i < symbol_table->length; i++) {
        curr_symbol = symbol_table->symbols[i];
        if (curr_symbol->type == ENTRY) {
            fprintf(ent_file, "%s: %d\n", curr_symbol->name, curr_symbol->val);
        }
    }
    fclose(ent_file);
    
    return 0;
}
