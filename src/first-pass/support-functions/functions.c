#include "../../error-handling/errors.h"
#include "../../globals/globals.h"
#include "../first-pass-headers/first_pass_headers.h"
#include "../first-pass-headers/instruction.h"

Symbol *find_symbol(Symbol *head, const char *name);
char *my_strdup(const char *str);

enum opcode get_opcode(const char *instruction_name_str) {
    int i = 0;
    while (commands[i].instruction_name != NULL) {
        if (strcmp(commands[i].instruction_name, instruction_name_str) == 0) {
            return commands[i].opcode_value;
        }
        i++;
    }
    return NULL_OP; /* If not found, return NULL_OP */
}

/* ENCODE FUNCTIONS */

/* IS A functions */
int is_register(const char *operand) {
    if (operand == NULL) {
        return 0; /* Return 0 if the operand pointer is NULL */
    }

    if (operand[0] == '\0') {
        return 0; /* Return 0 if the operand string is empty */
    }

    if (operand[0] == '@' && operand[1] == 'r' && isdigit(operand[2])) {
        if ((operand[2] - '0') <= 7) {
            return 1; /* Return 1 if the operand represents a valid register */
        }
    }
    return 0; /* Return 0 if the operand does not match the register format */
}
int is_number(const char *str) {
    if (str == NULL || *str == '\0') {
        return 0; /* Return 0 if the input pointer is NULL or if the string is empty */
    }

    /* Check for a plus (+) or minus (-) sign at the beginning of the string */
    if (*str == '+' || *str == '-') {
        str++; /* Move to the next character */
    }

    while (*str) {
        if (!isdigit(*str)) {
            return 0; /* Return 0 if a non-digit character is encountered */
        }
        str++; /* Move to the next character */
    }

    return 1; /* Return 1 if all characters are digits */
}
int is_symbol(const char *symbol) {
    size_t i = 1;

    if (is_reserved_keyword(symbol))
        return 0;

    if (symbol == NULL || strlen(symbol) > 31 || strlen(symbol) == 0) {
        return 0; /* Invalid if NULL, longer than 31 chars, or empty */
    }

    if (!isalpha(symbol[0])) {
        return 0; /* Symbol must start with a letter */
    }

    for (i; symbol[i] != '\0'; i++) {
        if (!isalnum(symbol[i])) {
            return 0; /* Invalid character in the symbol */
        }
    }

    return 1; /* The string satisfies the criteria for a valid symbol */
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

/* get functions */
int get_register_number(char *register_str) {
    int register_number;
    if (register_str == NULL || strlen(register_str) != 3) {
        /* Invalid input: register_str is NULL or not of length 3 */
        /* You can handle this case as needed (e.g., return an error code) */
        return -1; /* Return a default value or an error code */
    }

    if (register_str[0] != '@' || register_str[1] != 'r') {
        /* Invalid input: register_str does not start with '@r' */
        /* You can handle this case as needed (e.g., return an error code) */
        return -1; /* Return a default value or an error code */
    }

    /* Extract the register number from the string and convert it to an integer */
    register_number = register_str[2] - '0';

    /* Check if the register number is valid (0 to 7) */
    if (register_number < 0 || register_number > 7) {
        /* Invalid input: register number is out of range */
        /* You can handle this case as needed (e.g., return an error code) */
        return -1; /* Return a default value or an error code */
    }

    return register_number;
}

/* INSTRUCTION FUNCTIOS */

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
    char *directive = ".string";
    char quote = '\"';
    char *start = line;
    char *last_non_whitespace = NULL;
    char *ptr;
    char *opening_quote = strchr(start, '"');
    char *closing_quote = NULL;
    int i = 0;

    /* Skip leading whitespace */
    while (*start != '\0' && is_whitespace(*start)) {
        start++;
    }

    for (i; i < words_to_skip + 1; i++) {
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
        for (index; words_array[index][0] != '\0'; index += 2) {
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
        if (!is_symbol(words_array[index])) {
            *error_found = 1;
            handle_error(OnlySymbolsAllowed, line_number);
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

/* FORMAT LINE */
int is_whitespace(char c) {
    return c == ' ' || c == '\t';
}

/* valid line has 80 char + '\n' char */
int line_too_long(char *line) {
    if (strlen(line) <= 81)
        return 1;
    return 0;
}

void format_line(char source[LEN], char formatted_line[LEN]) {
    /* Get the length of the input line */
    int i = 0;
    int len = (int)strlen(source);

    /* Variables to keep track of the formatted line */
    int formatted_index = 0;
    int last_was_space = 1;

    /* Iterate through each character of the input line */
    for (i; i < len; i++) {
        char current_char = source[i];
        int is_space = is_whitespace(current_char);

        /* Handle spaces and tabs */
        if (is_space) {
            if (!last_was_space && formatted_index > 0 && formatted_line[formatted_index - 1] != ',' && formatted_line[formatted_index - 1] != ':') {
                formatted_line[formatted_index++] = ' '; /* Replace multiple spaces with a single space */
            }
            last_was_space = 1;
        }
        /* Handle commas and colons */
        else if (current_char == ',' || current_char == ':') {
            if (!last_was_space) {
                if (formatted_index > 0 && formatted_line[formatted_index - 1] != ',' && formatted_line[formatted_index - 1] != ':') {
                    formatted_line[formatted_index++] = ' '; /* Add a space before comma/colon if there wasn't one */
                }
            }

            formatted_line[formatted_index++] = current_char;
            formatted_line[formatted_index++] = ' '; /* Add a space after comma/colon */
            last_was_space = 1;
        }
        /* Handle regular characters */
        else {
            formatted_line[formatted_index++] = current_char;
            last_was_space = 0;
        }
    }

    /* Remove trailing whitespace, if any */
    if (formatted_index > 0 && is_whitespace(formatted_line[formatted_index - 1])) {
        formatted_index--;
    }

    /* Null-terminate the formatted string */
    formatted_line[formatted_index] = '\0';
}

void store_words(char line[], char resultArray[LEN][LEN]) {
    char *token;
    const char delimiter[] = " ";
    int index = 0;

    memset(resultArray, 0, sizeof(resultArray[0][0]) * LEN * LEN);

    token = strtok(line, delimiter);

    while (token != NULL && index < LEN - 1) {
        if (strlen(token) < LEN) {
            strncpy(resultArray[index], token, LEN - 1);
            resultArray[index][LEN - 1] = '\0';
            index++;
        }
        token = strtok(NULL, delimiter);
    }
}

void format_and_store_words(char line[LEN], char words_array[LEN][LEN]) {
    char formatted_line[LEN] = {0};
    format_line(line, formatted_line);
    store_words(formatted_line, words_array);
}

void update_variables(char **current_symbol_name, int *symbol_definition, int *line_number, int *index) {
    *(current_symbol_name) = NULL;
    *symbol_definition = 0;
    *line_number = *line_number + 1;
    *index = 0;
}

int is_comment(const char *line) {
    int index = 0;

    while (isspace(line[index])) {
        index++; /* Skip leading whitespace characters. */
    }

    return (line[index] == ';') ? 1 : 0;
}

/* Function to check if a given line is empty (contains only whitespace characters) */
int is_empty(const char *line) {
    while (*line != '\0') {
        /* Loop through each character in the line until the end of the string is reached. */

        if (!isspace((unsigned char)*line)) {
            /*
             * Check if the current character is not a whitespace character.
             * The `(unsigned char)` cast is used to handle potential negative char values correctly.
             */

            return 0; /* Return 0 immediately if a non-whitespace character is found. */
        }

        line++; /* Move to the next character in the line. */
    }

    return 1; /* Return 1 if the loop reaches the end of the line without finding a non-whitespace character. */
}
/* Function to skip a line if it is a comment or empty */
int comment_or_empty(char *line) {
    return (is_comment(line) || is_empty(line));
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

char *my_strdup(const char *str) {
    size_t length = strlen(str) + 1; /* Include space for the null terminator */
    char *duplicate = (char *)malloc(length);

    if (duplicate != NULL) {
        strcpy(duplicate, str);
    }

    return duplicate;
}