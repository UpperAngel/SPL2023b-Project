#include "first-pass-headers/symbol_table.h"

SymbolTable *init_table(int init_size) {
    SymbolTable *new_table = (SymbolTable *)malloc(sizeof(SymbolTable));
    int i;
    
    if (new_table == NULL) {
        return NULL; /* Memory allocation failure */
    }

    new_table->length = 0;
    new_table->size = init_size;
    new_table->symbols = (Symbol **)malloc(sizeof(Symbol *) * init_size);
    if (new_table->symbols == NULL) {
        free(new_table);
        return NULL; /* Memory allocation failure */
    }

    for (i = 0; i < init_size; i++) {
        new_table->symbols[i] = NULL;
    }

    return new_table;
}

int add_symbol(SymbolTable *table, const char *name, int val, SymbolType type) {
    int i;
    
    if (table == NULL || name == NULL) {
        return 0; /* Failure */
    }

    int index = -1;
    for (i = 0; i < table->size; i++) {
        if (table->symbols[i] == NULL) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        int new_size = table->size * 2;
        SymbolTable *updated_table = (SymbolTable *)realloc(table, sizeof(SymbolTable) + sizeof(Symbol *) * new_size);
        if (updated_table == NULL) {
            return 0; /* Memory allocation failure */
        }

        for (i = table->size; i < new_size; i++) {
            updated_table->symbols[i] = NULL;
        }

        table = updated_table;
        table->size = new_size;
    }

    table->symbols[index] = (Symbol *)malloc(sizeof(Symbol));
    if (table->symbols[index] == NULL) {
        return 0; /* Memory allocation failure */
    }

    table->symbols[index]->name = strdup(name);
    table->symbols[index]->val = val;
    table->symbols[index]->type = type;
    table->length++;

    return 1; /* Success */
}

Symbol *get_symbol_by_name(const SymbolTable *table, const char *name) {
    int i;
    for (i = 0; i < table->length; i++) {
        if (strcmp(name, table->symbols[i]->name) == 0) {
            return table->symbols[i];
        }
    }
    return NULL;
}

void free_symbol_table(SymbolTable *table) {
    int i;
    for (i = 0; i < table->length; i++) {
        free(table->symbols[i]->name);
        free(table->symbols[i]);
    }
    free(table);
}

void print_symbol(const Symbol *symbol) {
    printf("name: %s, value: %d \n", symbol->name, symbol->val);
}

void print_table(const SymbolTable *table) {
    int i;
    for (i = 0; i < table->length; i++) {
        printf("%d. ", i);
        print_symbol(table->symbols[i]);
    }
}