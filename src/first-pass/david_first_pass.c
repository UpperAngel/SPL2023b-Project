#include "/home/david/CLionProjects/My-project/Project main branch/src/FIRST ---PASS/header files/first_pass_headers.h"
#include "/home/david/CLionProjects/My-project/Project main branch/src/Error handling/handle_error.h"

// TODO make sure that every number is macro based


int is_string_directive(char *word) {
    if (strcmp(word,".string") == 0)
        return 1;
    return 0;
}

int is_data_directive(char *word){
    if (strcmp(word,".data") == 0)
        return 1;
    return 0;
}


int first_pass(FILE *am_file, struct InstructionStructure *instructions_array, struct DataStructure *data_array, struct symboltable) {
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
        if (is_symbol_definition(word)) {
            if (is_valid_symbol(word, line_number,&error_found)) {/* In this function we should output errors if found  */
                symbol_definition = 1;
                current_symbol_name = get_symbol_name(word); /* STR */
                word = words_array[++index];
            } else {
                continue;
            }
        }

        /* step 5 */    /*example: STR: .data 6,7,3,5,6,7,4  */
        if (is_string_directive(word) || is_data_directive(word)) {
            /* step 6 */
            if (symbol_definition) {
                if (symbol_is_in_symbolTable(symbol_table, current_symbol_name)){
                    error_found = 1;
                    handle_error(MultipleSymbolDefinition, line_number);
                    continue;
                }
                add_symbol(symbol_table, current_symbol_name, DC, DATA);
            }

            /* step 7 */    /*example:  .string "abcdefg"  */
            if ( is_string_directive(word) ) {
                /* assuming encode_data encodes the data to the array and returns number of lines encoded */
                if ( valid_string_directive(words_array, line_number, &error_found,symbol_definition) )
                    DC = DC + handle_string_directive(data_array, DC, words_array);
            } else { /* data directive */ /*example:  .data 1,3,4,5,6,2,4,5  */
                if (valid_data_directive(words_array, line_number, &error_found))
                    DC = DC + handle_data_directive(data_array, DC, words_array);/* assuming encode_string encodes the string to the array and returns number of lines encoded */
            }
            continue;
        }

        /* step 8 + 9*/   /* example:  M1: .extern STR,M2,M3  */
        if (is_extern_directive(word) || is_entry_directive(word)) {

            if (symbol_definition) {
                //TODO here we could output warning
            }

            if (is_extern_directive(word)) {
                if (valid_extern_directive(words_array, line_number, &error_found))
                    handle_extern_directive(words_array, symbol_table);
            } else {
                if (valid_entry_directive(words_array, line_number, &error_found))
                    handle_entry_directive(words_array, symbol_table); /* here should be special treatment of ENTRY */
            }
            continue;
        }

        /* step 11 */
        if (symbol_definition) {
            if (symbol_is_in_symbolTable(symbol_table, current_symbol_name)){
                error_found = 1;
                handle_error(MultipleSymbolDefinition, line_number);
                continue;
            }
            add_symbol(symbol_tabel, current_symbol_name, CODE, IC);
        }

        /* step 12 + 13 + 14 */  /* example: STR: mov 5,M2 */
        if (valid_instruction(words_array, line_number, &error_found) )
            IC = IC + handle_valid_instruction(words_array, instructions_array,IC, symbol_definition);
        else {/* the second word is not an instruction nor data/string directive than output error */
            /* here should be function that can find more specific errors */
            error_found = 1;
            handle_error(SomeErrorHere,line_number);
            continue;
        }

    }
    update_labelTable(label_table,IC);
    return 0; /* here would be a struct */
 }













