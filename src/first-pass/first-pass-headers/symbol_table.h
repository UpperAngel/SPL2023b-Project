#ifndef __SYMBOL_TABLE_H
#define __SYMBOL_TABLE_H

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
typedef struct symbol Symbol;
typedef struct symbolTable SymbolTable;

Symbol *get_symbol_by_name(SymbolTable *table, const char *name);

void free_symbol_table(SymbolTable *table);

int add_symbol(SymbolTable *table, const char *name, const char *val);

SymbolTable *init_table(const int init_size);
#endif