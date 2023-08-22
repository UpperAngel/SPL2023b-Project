#include "../first-pass-headers/first_pass_headers.h"
#include "../../error-handling/errors.h"
int is_directive(char *word, const char *directive) {
    if (strcmp(word, directive) == 0) {
        return 1;
    }
    return 0;
}
/* By the project definition valid characters are visible ASCI chars which are chars between 32 and 126 including in the ASCI table */
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
    if (*start != quote){
        *error_found = 1;
        handle_error(NonWhiteCharsBeforeQuote,line_number);
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
void write_character(struct DataStructure* data, char c) {
    /* Convert the char to int and store it in the struct */
    data->value = (int)c;
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
void write_integer(struct DataStructure* data, int number) {
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
        handle_error(CommaAtStart,line_number);  /* Comma at start */
        return 0;  /* Invalid */
    }

    i++;  /* Move to the next word */

    /* Iterate through the words */
    while (words_array[i][0] != '\0') {
        /* Check if the current word is a comma */
        if (words_array[i][0] == ',') {
            /* Check for consecutive commas */
            if (words_array[i - 1][0] == ',') {
                handle_error(ConsecutiveCommas,line_number);  /* 2 consecutive commas */
                return 0;  /* Invalid */
            }
        } else {
            /* Check if the previous word is not a comma */
            if (words_array[i - 1][0] != ',') {
                handle_error(MissingComma,line_number);  /* Comma expected after a word */
                return 0;  /* Invalid */
            }
        }

        i++;  /* Move to the next word */
    }

    /* Check if the last word is a comma */
    if (words_array[i - 1][0] == ',') {
        handle_error(EndsWithComma,line_number);  /* Ends with comma error */
        return 0;  /* Invalid */
    }

    return 1;  /* Valid */
}
int valid_data_directive(char words_array[LEN][LEN], int line_number, int *error_found, int symbol_definition) {
    int index = 1; /* Start IC_index for checking words_array */

    /* Adjust index if symbol is defined */
    if (symbol_definition) {
        index = index + 2;
    }

    /* Check if commas are valid in the line */
    if (valid_commas_in_directive(words_array, index, line_number) ) {
        /* Iterate through words_array in steps of 2 */
        for (index; words_array[index][0] != '\0'; index += 2) {
            /* Check if the current word is not a number */
            if (!is_number(words_array[index])) {
                handle_error(InvalidCharInDataDirective, line_number);
                *error_found = 1; /* Set error flag */
                return 0;         /* Directive is not valid */
            }

            /* Check the number range */
            if (!valid_number_range(words_array[index]) ) {
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



