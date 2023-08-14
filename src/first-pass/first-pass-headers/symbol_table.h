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
    ENTRY,
    NONE = -1,
} SymbolType;

Symbol *get_symbol_by_name(const SymbolTable *table, const char *name);

void free_symbol_table(SymbolTable *table);

int add_symbol(SymbolTable *table, const char *name, int val, SymbolType type);

SymbolTable *init_table(const int init_size);


void print_symbol(const Symbol *);
void print_table(const SymbolTable *);
#endif