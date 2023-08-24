#include "../first-pass-headers/first_pass_headers.h"

Symbol *find_symbol(Symbol *head, const char *name);

/* DIRECTIVE HANDLING FUNCTIONS */
int is_directive(char *word, const char *directive) {
    if (strcmp(word, directive) == 0) {
        return 1;
    }
    return 0;
}
int is_valid_char(char c) {
    return (c >= 32 && c <= 126);
}
char *custom_strrchr(const char *str, int ch) {
    const char *last = NULL;

    while (*str != '\0') {
        if (*str == ch) {
            last = str;
        }
        str++;
    }

    return (char *)last;
}
int valid_string_directive(char *line, int line_number, int *error_found, int words_to_skip) {
    char quote = '\"';
    char *start = line;
    char *ptr;
    char *opening_quote = strchr(start, '"');
    char *closing_quote = NULL;
    int i = 0;

    /* Skip leading whitespace */
    while (*start != '\0' && is_whitespace(*start)) {
        start++;
    }

    for (i = 0; i < words_to_skip + 1; i++) {
        /* skip first word and second word */
        while (*start != '\0' && !is_whitespace(*start)) {
            start++;
        }

        if (*start == '\0') {
            *error_found = 1;
            handle_error(InvalidFormat, line_number);
            return 0;
        }

        /* Skip whitespace between words */
        while (*start != '\0' && is_whitespace(*start)) {
            start++;
        }
    }

    /* Missing opening quote */
    if (!opening_quote) {
        *error_found = 1;
        handle_error(MissingStartOfStringSign, line_number);
        return 0;
    }

    /* Check characters before the directive */
    if (*start != quote) {
        *error_found = 1;
        handle_error(NonWhiteCharsBeforeQuote, line_number);
        return 0;
    }

    /* Check characters within the string */
    closing_quote = custom_strrchr(opening_quote + 1, '"');
    if (!closing_quote) {
        *error_found = 1;
        handle_error(MissingEndOfStringSign, line_number);
        return 0;
    }

    for (ptr = opening_quote + 1; ptr < closing_quote; ptr++) {
        if (!is_valid_char(*ptr)) {
            *error_found = 1;
            handle_error(InvalidCharInString, line_number);
            return 0;
        }
    }

    /* Check the characters after closing quote */
    char *after_closing_quote = closing_quote + 1;
    while (*after_closing_quote != '\0') {
        if (!is_whitespace(*after_closing_quote)) {
            *error_found = 1;
            handle_error(NonWhiteCharsAfterClosingQuote, line_number);
            return 0;
        }
        after_closing_quote++;
    }
    return 1;
}

int handle_string_directive(struct DataStructure *data_array, int DC, char *line, int line_number) {
    char *start = line;
    char *closing_quote = custom_strrchr(start, '"'); /* Find the last quotation mark using custom_strrchr */
    int chars_inserted = 0;
    /* Skip to the first opening quote */
    while (*start != '\0' && *start != '"') {
        start++;
    }
    start++; /* Move past the opening quote */

    while (start < closing_quote) {
        write_character(&data_array[DC], *start);
        DC++;
        chars_inserted++;
        start++;
    }

    /* Insert null terminator at the end of the string */
    write_character(&data_array[DC], '\0');
    chars_inserted++;

    return chars_inserted;
}

void write_integer(struct DataStructure *data, int number) {
    data->value = number & 0xFFF; /* Mask the value to keep only the 12 least significant bits (LSBs) */
}

int valid_number_range(char *word) {
    int value = atoi(word);

    /* valid 12 bits integer values */
    if (value < -2048 || value > 2047) {
        return 0;
    }

    return 1; /* No error */
}

int valid_commas_in_directive(char words_array[LEN][LEN], int starting_index, int line_number) {
    int i = starting_index;

    /* Check if the first word is a comma */
    if (words_array[i][0] == ',') {
        handle_error(CommaAtStart, line_number); /* Comma at start */
        return 0;                                /* Invalid */
    }

    i++; /* Move to the next word */

    /* Iterate through the words */
    while (words_array[i][0] != '\0') {
        /* Check if the current word is a comma */
        if (words_array[i][0] == ',') {
            /* Check for consecutive commas */
            if (words_array[i - 1][0] == ',') {
                handle_error(ConsecutiveCommas, line_number); /* 2 consecutive commas */
                return 0;                                     /* Invalid */
            }
        } else {
            /* Check if the previous word is not a comma */
            if (words_array[i - 1][0] != ',') {
                handle_error(MissingComma, line_number); /* Comma expected after a word */
                return 0;                                /* Invalid */
            }
        }

        i++; /* Move to the next word */
    }

    /* Check if the last word is a comma */
    if (words_array[i - 1][0] == ',') {
        handle_error(EndsWithComma, line_number); /* Ends with comma error */
        return 0;                                 /* Invalid */
    }

    return 1; /* Valid */
}

int valid_data_directive(char words_array[LEN][LEN], int line_number, int *error_found, int symbol_definition) {
    int index = 1; /* Start IC_index for checking words_array */

    /* Adjust index if symbol is defined */
    if (symbol_definition) {
        index = index + 2;
    }

    /* Check if commas are valid in the line */
    if (valid_commas_in_directive(words_array, index, line_number)) {
        /* Iterate through words_array in steps of 2 */
        for (index = 0; words_array[index][0] != '\0'; index += 2) {
            /* Check if the current word is not a number */
            if (!is_number(words_array[index])) {
                handle_error(InvalidCharInDataDirective, line_number);
                *error_found = 1; /* Set error flag */
                return 0;         /* Directive is not valid */
            }

            /* Check the number range */
            if (!valid_number_range(words_array[index])) {
                handle_error(IntegerOverflow, line_number);
                *error_found = 1; /* Set error flag */
                return 0;         /* Directive is not valid */
            }
        }
    } else {
        *error_found = 1; /* Set error flag */
        return 0;         /* Directive is not valid */
    }

    return 1; /* Directive is valid */
}

int handle_data_directive(struct DataStructure *data_array, int DC, char words_array[LEN][LEN], int symbol_definition) {
    int data_index = 1;
    int values_inserted = 0;

    /* if there is a symbol then we should skip 2 words because words_array separates symbol name and the char ':'  */
    if (symbol_definition)
        data_index = data_index + 2;

    while (words_array[data_index][0] != '\0') {
        int value = atoi(words_array[data_index]);

        write_integer(&data_array[DC], value);
        values_inserted++;
        DC++;

        /* Move to the next word if it's a comma */
        if (words_array[data_index + 1][0] == ',') {
            data_index += 2; /* Skip the comma and move to the next word */
        } else {
            data_index++; /* Move to the next word */
        }
    }

    return values_inserted;
}

/* EXTERN AND ENTRY FUNCTIONS */
int valid_entry_and_extern_directive(char words_array[LEN][LEN], int *error_found, int line_number, int symbol_definition) {
    int index = 1;
    /* if there is symbol definition skip the name of the symbol and ':' char */
    if (symbol_definition)
        index = index + 2;

    if (!valid_commas_in_directive(words_array, index, line_number)) {
        *error_found = 1;
        return 0;
    }

    if (words_array[index][0] == '\0') {
        *error_found = 1;
        handle_error(InvalidNumberOfOperands, line_number);
        return 0;
    }

    while (words_array[index][0] != '\0') {
        sanitize_input(words_array[index]);
        if (!is_symbol(words_array[index])) {
            *error_found = 1;
            handle_error(OnlySymbolsAllowed, line_number);
            printf("the symbol - '%s'", words_array[index]);
            return 0;
        }
        /* check the next parameter, skipping the: ',' char */
        index = index + 2;
    }
    /* passed all checks */
    return 1;
}

void handle_extern_and_entry_directives(char words_array[LEN][LEN], Symbol **symbol_head, int symbol_definition, int line_number, int *error_found, SymbolType type, SymbolCategory category) {
    int index = 1;

    if (symbol_definition)
        index = index + 2;

    while (words_array[index][0] != '\0') {
        handle_symbol(symbol_head, words_array[index], line_number, error_found, category, type, 0);
        index = index + 2;
    }
}

/* SYMBOL STRUCT FUNTIONS */
void handle_symbol(Symbol **head, const char *name, int line_number, int *error_found, SymbolType parameter_type, SymbolCategory parameter_entry_or_extern, int parameter_value) {
    Symbol *existing_symbol = NULL;
    Symbol *new_symbol = NULL;
    if (head == NULL || name == NULL) {
        return; /* Safeguard against invalid inputs */
    }

    /* Check if symbol with the given name already exists */
    existing_symbol = find_symbol(*head, name);
    if (existing_symbol != NULL) {
        /* mark existing symbol as extern or entry */
        switch (existing_symbol->category) {
            case (NONE_CATEGORY):
                if (parameter_entry_or_extern == ENTRY)
                    existing_symbol->type = ENTRY;
                else if (parameter_entry_or_extern == EXTERN) {
                    handle_error(RedefinitionOfSymbolType, line_number);
                    *error_found = 1;
                    return;
                } else if (parameter_entry_or_extern == NONE_CATEGORY) {
                    handle_error(MultipleSymbolDefinition, line_number);
                    *error_found = 1;
                    return;
                }
                break;
            case (ENTRY):
                if (parameter_entry_or_extern == EXTERN) {
                    handle_error(RedefinitionOfSymbolType, line_number);
                    *error_found = 1;
                    return;
                } else if (existing_symbol->val == 0 && parameter_entry_or_extern == NONE_CATEGORY) {
                    existing_symbol->val = parameter_value;
                } else if (parameter_entry_or_extern == ENTRY) {
                } /* do nothing */
                else {
                    handle_error(MultipleSymbolDefinition, line_number);
                    *error_found = 1;
                    return;
                }
                break;
            case (EXTERN):
                if (parameter_entry_or_extern == ENTRY) {
                    handle_error(RedefinitionOfSymbolType, line_number);
                    *error_found = 1;
                    return;
                } else if (parameter_entry_or_extern == EXTERN) {
                } /* do nothing */
                else {
                    handle_error(RedefinitionOfExternSymbol, line_number);
                    *error_found = 1;
                    return;
                }
                break;
        }
    }
    /* Create a new symbol and handle memory allocation errors */
    new_symbol = create_symbol(name, parameter_value, parameter_entry_or_extern, parameter_type);
    if (new_symbol == NULL) {
        handle_error(FailedToAllocateMemory, line_number);
        *error_found = 1;
        return;
    }
    new_symbol->next = *head; /* Link the new symbol to the current head */
    *head = new_symbol;       /* Set head to the new symbol */
}
