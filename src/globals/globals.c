#include "globals.h"

const char *reservedKeywords[RESERVED_KEYWORDS_LENGTH] = {
        "mov", "cmp", "add", "sub", "not", "clr", "lea", "inc",
        "de", "jmp", "bne", "red", "prn", "jsr", "rts", "stop"
};

FILE *create_file(const char *file_name, const char *extension) {
  FILE *created_file = NULL;
  
  char *new_file_name;
  char *dot = NULL;

  /* dynamiclly allocate memory for the file name and the extension */
  new_file_name = (char *)malloc(strlen(file_name) +strlen(extension) + 1);
  if (new_file_name == NULL)
  {
    printf("faild allocation for the file name \n");
    return NULL;
  }
  
  /* copy the input to file_name */
  strcpy(new_file_name, file_name);
  
  /* deletes the file extension. everything after the dot and the dot itself */
  dot = strchr(new_file_name, '.');
  if (dot != NULL)
  {
    *dot = '\0';
  }
  /* adds the extension and creates the file*/
  new_file_name = strcat(new_file_name, extension);
  created_file = fopen(new_file_name, "w+");

  /* checks if file was created succesfully */
  if (created_file == NULL)
  {
    printf("failed to create the file: \"%s\" \n", new_file_name);
    return NULL;
  }
  free(new_file_name);

  /*  returns the file after being created*/
  return created_file;
}

char *my_strdup(const char *str) {
    size_t length = strlen(str) + 1; /* Include space for the null terminator */
    char *duplicate = (char *)malloc(length);

    if (duplicate != NULL) {
        strcpy(duplicate, str);
    }

    return duplicate;
}


int is_reserved_keyword(const char *word) {
    int i = 0;
    for (i = 0; i < sizeof(reservedKeywords) / sizeof(reservedKeywords[0]); i++) {
        if (strcmp(word, reservedKeywords[i]) == 0) {
            return 1; /* Found a match, it's a reserved keyword */
        }
    }
    return 0; /* No match, it's not a reserved keyword */
}

struct SymbolNameAndIndex* create_node(const char *name, int IC_index, int line_number) {
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


struct SymbolNameAndIndex *get_node_by_name(struct SymbolNameAndIndex *head, const char *target_name) {
    while (head != NULL) {
        if (strcmp(head->name, target_name) == 0) {
            return head;
        }
        head = head->next;
    }
    return NULL; /* Node not found */
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