#include "globals.h"
#include "../error-handling/errors.h"


struct SymbolNameAndIndex* createNode(const char *name, int IC_index, int line_number) {
    struct SymbolNameAndIndex* newNode = malloc(sizeof(struct SymbolNameAndIndex));
    if (newNode == NULL) {
        handle_error(FailedToAllocateMemory, line_number);
        return NULL;
    }

    newNode->name = my_strdup(name);  /* Duplicate the string to avoid issues */
    newNode->IC_index = IC_index;
    newNode->line_number = line_number;  /* Set line number */
    newNode->next = NULL;
    return newNode;
}
/* Function to insert a node at the end of the list */
void insertNode(struct SymbolNameAndIndex** head, struct SymbolNameAndIndex* newNode) {
    if (*head == NULL) {
        *head = newNode;
        return;
    }

    struct SymbolNameAndIndex* current = *head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = newNode;
}
/* Function to free the linked list and its nodes */
void freeList(struct SymbolNameAndIndex* head) {
    struct SymbolNameAndIndex* current = head;
    while (current != NULL) {
        struct SymbolNameAndIndex* temp = current;
        current = current->next;
        free(temp);  /* Deallocate the memory for the current node, including the name string */
    }
}
/* Function to get the name at a specified index */
char* get_name_at(const struct SymbolNameAndIndex* head, int targetIndex) {
    const struct SymbolNameAndIndex* current = head;
    while (current != NULL) {
        if (current->IC_index == targetIndex) {
            return current->name;
        }
        current = current->next;
    }
    return NULL;  /* Index not found */
}
/* Function to get the index at a specified index */
int get_index_at(const struct SymbolNameAndIndex* head, int targetIndex) {
    const struct SymbolNameAndIndex* current = head;
    while (current != NULL) {
        if (current->IC_index == targetIndex) {
            return current->IC_index;
        }
        current = current->next;
    }
    return -1;  /* Index not found */
}
/* Function to get the line number at a specified index */
int get_line_number_at(const struct SymbolNameAndIndex* head, int targetIndex) {
    const struct SymbolNameAndIndex* current = head;
    while (current != NULL) {
        if (current->IC_index == targetIndex) {
            return current->line_number;
        }
        current = current->next;
    }
    return -1;   /* Index not found */
}

void write_character(struct DataStructure *data, char c) {
    /* Convert the char to int and store it in the struct */
    data->value = (int)c;
}



int is_valid_symbol(const char *line, int line_number, int *error_found, char words_array[LEN][LEN]) {
    /* Skip initial white spaces */
    while (isspace(*line)) {
        line++;
    }

    /*  Check if the first character is a letter */
    if (!isalpha(*line)) {
        handle_error(SymbolMustStartWithLetter, line_number);
        *error_found = 1;
        return 0;
    }

    /*  Check if the rest of the characters are letters or digits */
    line++;
    while (isalnum(*line)) {
        line++;
    }

    /*  Check for ':' after the symbol name */
    if (*line != ':') {
        *error_found = 1;
        handle_error(SymbolMustEndWithColon, line_number);
        return 0;
    }

    if (is_reserved_keyword(words_array[0])) {
        *error_found = 1;
        handle_error(SymbolIsReservedWord, line_number);
        return 0;
    }

    if (words_array[2] == '\0') {
        *error_found = 1;
        handle_error(EmptySymbolDefinition, line_number);
        return 0;
    }

    return 1;
}


Symbol *find_symbol(Symbol *head, const char *name) {
    Symbol *current = head;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL; /* Symbol not found */
}
int is_in_symbol_list(Symbol *head, const char *name) {
    Symbol *symbol = find_symbol(head, name);
    return (symbol != NULL);
}
Symbol *create_symbol(const char *name, int val, SymbolType type, SymbolCategory category) {
    Symbol *new_symbol = (Symbol *)malloc(sizeof(Symbol));
    if (new_symbol != NULL) {
        new_symbol->name = my_strdup(name); /* Allocate and copy name */
        new_symbol->val = val;
        new_symbol->type = type;
        new_symbol->category = category;
        new_symbol->next = NULL;
    }
    return new_symbol;
}

void free_symbol_list(Symbol *head) {
    while (head != NULL) {
        Symbol *temp = head;
        head = head->next;
        free(temp->name);
        free(temp);
    }
}