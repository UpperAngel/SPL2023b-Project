#include "first-pass-headers/symbol_table.h"



struct symbol {
    char *name;
    char *val;
    SymbolType type;

};

struct symbolTable {
    int length;
    int size;
    Symbol *symbols[];
};


SymbolTable *init_table(const int init_size) {
    int i;
    SymbolTable *new_table = NULL;

    new_table = (SymbolTable *)malloc(sizeof(SymbolTable) + sizeof(Symbol *) * init_size);
    new_table->length = 0;  
    new_table->size = init_size;

    for (i = 0; i < init_size; i++) {
        new_table->symbols[i] = NULL;
    }

    return new_table;
}

int add_symbol(SymbolTable *table, const char *name, const char *val, SymbolType type) {
    int i;
    int new_len;
    int index = -1;
    SymbolTable *updated_table;

    /* if the table or symbol or value is null, we return 0 as a failure */
    if (table == NULL || name == NULL || val == NULL) {
        return 0; /* failure */
    }

    /* found a free index */
    for (i = 0; i < table->size; i++) {
        if (table->symbols[i] == NULL) {
            index = i;
            break;
        }
    }

    /* the table is full, we resize it to be 2 times as big */
    if (index == -1) {
        new_len = table->size * 2;
        updated_table = realloc((void *)table, sizeof(SymbolTable) + sizeof(Symbol) * new_len);

        /* memory allocation failed, we return 0 */
        if (updated_table == NULL) {
            return 0;
        }

        /* initialization of the new symbols */
        for (i = table->size; i < new_len; i++) {  // Corrected: Use table->size instead of table->length
            updated_table->symbols[i] = NULL;
        }

        table = updated_table;
        table->size = new_len;  // Corrected: Update the size, not the length
    }

    table->symbols[index] = (Symbol *)malloc(sizeof(Symbol));
    table->symbols[index]->name = (char *)malloc(strlen(name) + 1);  // Corrected: Use strlen(sym) + 1
    table->symbols[index]->val = (char *)malloc(strlen(val) + 1);  // Corrected: Use strlen(val) + 1

    table->length += 1;  /* Increment the length */

    strcpy(table->symbols[index]->name, name);
    strcpy(table->symbols[index]->val, val);
    table->symbols[index]->type = type;

    return 1;
}

Symbol *get_symbol_by_name(SymbolTable *table, const char *name) {
    int i;
    for (i = 0; i < table->length; i++)
    {
        if (strcmp(name, table->symbols[i]->name) == 0)
        {
            return table->symbols[i];
        }
        
    }
    return NULL;
    
}




void free_symbol_table(SymbolTable *table) {
    int i;
    for (i = 0; i < table->length; i++) {
        free(table->symbols[i]->name);
        free(table->symbols[i]->val);
        free(table->symbols[i]);
    }
    free(table);
}

