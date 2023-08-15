#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    CODE,
    DATA,
    EXTERN,
    ENTRY,
    NONE = -1
} SymbolType;

typedef struct {
    char *name;
    int val;
    SymbolType type;
} Symbol;

typedef struct {
    int length;
    int size;
    Symbol **symbols;
} SymbolTable;

Symbol *get_symbol_by_name(const SymbolTable *table, const char *name);

void free_symbol_table(SymbolTable *table);

int add_symbol(SymbolTable *table, const char *name, int val, SymbolType type);

SymbolTable *init_table(int init_size);

void print_symbol(const Symbol *symbol);

void print_table(const SymbolTable *table);

#endif