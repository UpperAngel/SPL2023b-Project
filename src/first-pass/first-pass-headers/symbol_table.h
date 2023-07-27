#ifndef __SYMBOL_TABLE_H
#define __SYMBOL_TABLE_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct symbol Symbol;
typedef struct symbolTable SymbolTable;
typedef enum symbolType {
    CODE,
    DATA,
    EXTERN,
    NONE = -1,
} SymbolType;

Symbol *get_symbol_by_name(SymbolTable *table, const char *name);

void free_symbol_table(SymbolTable *table);

int add_symbol(SymbolTable *table, const char *name, const char *val, SymbolType type);

SymbolTable *init_table(const int init_size);
#endif