#include <stdio.h>
#include "src/second-pass/symbolNameAndIndex.h"
#include "src/second-pass/symbol_list.h"
#include "src/pre-processor/pre-processor header files/helper_fucntions.h"

int main() {
    // Create some example symbols and add them to the symbol table
    struct Symbol *symbol_table = NULL;
    struct Symbol *entry_symbol = (struct Symbol *)malloc(sizeof(struct Symbol));
    entry_symbol->name = strdup("entry_symbol");
    entry_symbol->val = 100;
    entry_symbol->type = ENTRY;
    entry_symbol->next = NULL;
    symbol_table = entry_symbol;

    // Add more symbols to symbol_table with different names
    for (int i = 0; i < 10; i++) {
        char name[20];
        snprintf(name, sizeof(name), "symbol_%d", i);
        struct Symbol *new_symbol = (struct Symbol *)malloc(sizeof(struct Symbol));
        new_symbol->name = strdup(name);
        new_symbol->val = i * 50;
        new_symbol->type = i % 2 == 0 ? EXTERN : ENTRY;
        new_symbol->next = symbol_table;
        symbol_table = new_symbol;
    }

    // Create some example SymbolNameAndIndex nodes and add them to the list
    struct SymbolNameAndIndex *symbol_name_and_index = NULL;
    struct SymbolNameAndIndex *node1 = createNode("entry_symbol", 5, 1);
    insertNode(&symbol_name_and_index, node1);

    // Add more nodes to symbol_name_and_index with different names
    for (int i = 0; i < 10; i++) {
        char name[20];
        snprintf(name, sizeof(name), "symbol_%d", i);
        insertNode(&symbol_name_and_index, createNode(name, i * 5, i + 2));
    }

    // Create files (ob_file, ent_file, ext_file)
    FILE *ent_file = fopen("entry_file.txt", "w");
    FILE *ext_file = fopen("extern_file.txt", "w");

    // Loop through the symbol table and process the nodes
    struct SymbolNameAndIndex *temp = symbol_name_and_index;
    while (symbol_table != NULL) {
        if (symbol_table->type == ENTRY) {
            fprintf(ent_file, "%s: %d\n", symbol_table->name, symbol_table->val);
        }

        temp = symbol_name_and_index;
        while (temp != NULL) {
            if (symbol_table->type == EXTERN) {
                fprintf(ext_file, "%s: %d\n", symbol_table->name, getNodeByName(temp, symbol_table->name) != NULL ? temp->IC_index : -1);
            }
            temp = temp->next;
        }

        symbol_table = symbol_table->next;
    }

    // Close the files
    fclose(ent_file);
    fclose(ext_file);

    // Free allocated memory for symbols and nodes
    struct Symbol *current_symbol = symbol_table;
    while (current_symbol != NULL) {
        struct Symbol *temp_symbol = current_symbol;
        current_symbol = current_symbol->next;
        free(temp_symbol->name);
        free(temp_symbol);
    }
    freeList(symbol_name_and_index);

    return 0;
}



