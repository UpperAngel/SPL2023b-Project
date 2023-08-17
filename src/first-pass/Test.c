#include "/home/david/CLionProjects/My-project/Project main branch/src/FIRST ---PASS/header files/first_pass_headers.h"
#include "/home/david/CLionProjects/My-project/Project main branch/src/Error handling/errors.h"

/* TESTING HERE */

int is_reserved_keyword(const char *word) {
    for (size_t i = 0; i < sizeof(reservedKeywords) / sizeof(reservedKeywords[0]); i++) {
        if (strcmp(word, reservedKeywords[i]) == 0) {
            return 1; /* Found a match, it's a reserved keyword */
        }
    }
    return 0; /* No match, it's not a reserved keyword */
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

    for ( i; symbol[i] != '\0'; i++) {
        if ( !isalnum(symbol[i]) ){
            return 0; /* Invalid character in the symbol */
        }
    }

    return 1; /* The string satisfies the criteria for a valid symbol */
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
int valid_entry_and_extern_directive(char words_array[LEN][LEN], int *error_found, int line_number, int symbol_definition){
    int index = 1;

    /* if there is symbol definition skip the name of the symbol and ':' char */
    if(symbol_definition)
        index = index + 2;

    if( !valid_commas_in_directive(words_array,index,line_number) ){
        *error_found = 1;
        return 0;
    }

    if (words_array[index][0] == '\0'){
        *error_found = 1;
        handle_error(InvalidNumberOfOperands,line_number);
        return 0;
    }

    while ( words_array[index][0] != '\0'){
        if( !is_symbol(words_array[index]) ){
            *error_found = 1;
            handle_error(OnlySymbolsAllowed,line_number);
            return 0;
        }
        /* check the next parameter, skipping the: ',' char */
        index = index + 2;
    }
    /* passed all checks */
    return 1;
}




void first_pass(FILE *am_file, struct InstructionStructure *instructions_array, struct DataStructure *data_array,  Symbol *symbol_head, struct SymbolNameAndIndex **second_pass_list ) {
    char line[LEN];
    char words_array [LEN][LEN] = {0}; /* Initialize array */
    int index = 0;
    int symbol_definition = 0;
    int error_found = 0;
    int IC = 0;
    int DC = 0;
    int line_number = 0;
    char *word = NULL;
    char *current_symbol_name = NULL;


    while ( fgets(line,LEN ,am_file) ) {
        update_variables(&current_symbol_name, &symbol_definition,&line_number,&index);

        /* Check if the current line is a comment or an empty line if so skip it */
        if(comment_or_empty(line))
            continue;

        if (line_too_long(line)) {
            handle_error(LineLimitExceeded, line_number);
            error_found = 1;
            continue;
        }
        /* breaks the lines to separate words and stores them into words_array */
        format_and_store_words(line,words_array);
        word = words_array[index];

        /* Step 3 and 4 */
        if ( words_array[index + 1 ][0] == ':' ) {
            if ( is_valid_symbol(line, line_number,&error_found, words_array) ) {/* In this function we should output errors if found  */
                symbol_definition = 1;
                current_symbol_name = word; /* STR */
                index = index + 2; /* skip the name of the symbol and the char ':' */
                word = words_array[index];
            } else {
                continue;
            }
        }

        /* step 5 */    /*example: STR: .data 6,7,3,5,6,7,4  */
        if (is_directive(word,".string") || is_directive(word,".data")) {
            /* step 6 */
            if (symbol_definition)
                handle_symbol(&symbol_head, current_symbol_name, line_number, &error_found, DATA , NONE_CATEGORY, DC);
            /* if there was an error in with the symbol handling continue to next line */
            if(error_found)
                continue;

            /* step 7 */    /*example:  STR: .string     "abc , de fg"  */
            if ( is_directive(word,".string") ) {
                if ( valid_string_directive(line, line_number, &error_found,symbol_definition ) )
                    DC = DC + handle_string_directive(data_array, DC,line, line_number);
            } else { /* data directive */
                if ( valid_data_directive(words_array, line_number, &error_found,symbol_definition) )
                    DC = DC + handle_data_directive(data_array, DC, words_array, symbol_definition );
            }
            continue;
        }

        /* step 8 + 9*/   /*   M1: .extern STR,M2,M3  */
        if (is_directive(word,".extern") || is_directive(word,".entry")) {
            if ( !valid_entry_and_extern_directive(words_array, &error_found, line_number, symbol_definition) )
                continue;

            if (is_directive(word,".extern") )
                handle_extern_and_entry_directives(words_array,&symbol_head,symbol_definition,line_number, &error_found,NONE_TYPE,EXTERN);
            else
                handle_extern_and_entry_directives(words_array,&symbol_head,symbol_definition,line_number, &error_found, NONE_TYPE,ENTRY);
        }


        /* step 11 */
        if (symbol_definition)
            handle_symbol(&symbol_head, current_symbol_name,line_number,&error_found, CODE, NONE_CATEGORY, IC);

        /* step 12 + 13 + 14 */  /* example: STR: mov 5,M2 */
        if (valid_instruction(words_array, line_number, symbol_definition) )
            IC = IC + handle_valid_instruction(words_array, instructions_array,IC, symbol_definition, line_number, second_pass_list );
        else {/* the second word is not an instruction nor data/string directive than output error */
            /* here should be a function that can find more specific errors */
            error_found = 1;
            //handle_error(SomeErrorHere,line_number);
            continue;
        }
    }
    //handle_separation(symbol_head,IC,line_number,&error_found);
}



int main() {
    // Create a mock FILE pointer for testing purposes
    FILE *mock_file = fopen("test_input.txt", "r");

    // Define your instruction and data arrays, symbol head, and second pass list
    struct InstructionStructure instructions_array[100];
    struct DataStructure data_array[100];
    Symbol *symbol_head = NULL;
    struct SymbolNameAndIndex *second_pass_list = NULL;

    // Call the first_pass function with the mock FILE pointer and other required arguments
    first_pass(mock_file, instructions_array, data_array, symbol_head, &second_pass_list);

    // Close the mock FILE pointer after use
    fclose(mock_file);

    // Print the results or perform further tests/assertions

    return 0;
}



