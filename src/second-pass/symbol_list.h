#include <stdint.h>
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

typedef struct Symbol {
    char *name;
    int val;
    SymbolType type;
    struct Symbol *next;
} Symbol;





Symbol *create_symbol(const char *name, int val, SymbolType type) {
    Symbol *new_symbol = (Symbol *)malloc(sizeof(Symbol));
    if (new_symbol == NULL) {
        /* Handle memory allocation error */
        return NULL;
    }

    new_symbol->name = strdup(name); /* Allocate and copy name */
    if (new_symbol->name == NULL) {
        /* Handle memory allocation error */
        free(new_symbol);
        return NULL;
    }

    new_symbol->val = val;
    new_symbol->type = type;
    new_symbol->next = NULL;

    return new_symbol;
}

void add_symbol_to_list(Symbol **head, Symbol *new_symbol) {
    if (new_symbol == NULL) {
        return; /* No symbol to add */
    }

    if (*head == NULL) {
        *head = new_symbol; /* List is empty, new_symbol becomes the head */
    } else {
        Symbol *current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_symbol; /* Add new_symbol at the end of the list */
    }
}

Symbol *find_symbol(Symbol *head, const char *name) {
    Symbol *current = head;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL; // Symbol not found
}

int is_in_symbol_list(Symbol *head, const char *name) {
    Symbol *symbol = find_symbol(head, name);
    return (symbol != NULL);
}



void free_symbol_list(Symbol *head) {
    while (head != NULL) {
        Symbol *temp = head;
        head = head->next;
        free(temp->name);
        free(temp);
    }
}
