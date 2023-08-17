#include "pre-processor header files/pre_processor.h"
#include "handle_error.h"


/* Function to process the source file and generate the target file
 In the case that a given line exceeds 80 chars the function will output to the target file up to 100 chars of
 that line and the error will be handled in the first pass  */
int process_file(FILE* source_file, FILE* target_file) {

    int line_number = 0; /* Variable to keep track of the line number being processed in the source file */
    enum MacroState state = OUTSIDE_MACRO_DEF; /* Initialize the state of the macro processing to be outside a macro definition */
    char line[MAXLEN]; /* Buffer to store each line read from the source file */
    char curr_macro_name[MACRO_NAME_LENGTH + 1]; /* Buffer to store the name of the current macro being processed */
    mcro* created_macro = NULL; /* Pointer to the macro structure representing the macro being created */
    struct macroList* macro_list = createMacroList(line_number); /* Pointer to the macro list containing all defined macros */


    while (custom_fgets(line, MAXLEN, source_file) != NULL) {
        line_number++; /* Increment the number of the line */

        /* Remove newline character at the end of a string */
        remove_newline(line);

       /* Check if the current line is a comment or an empty line if so skip it */
        if(comment_or_empty(line))
            continue;

        if ( is_start_macro_def(line)){
            if ( !valid_start_macro_def(line, line_number) )
                return 0; /* Error found */
            state = INSIDE_MACRO_DEF;
            strcpy(curr_macro_name, get_second_token(line));
            created_macro = create_mcro(curr_macro_name, line_number);
            add_to_macro_table(created_macro, &macro_list, line_number);
            continue;
        }else if (is_end_macro_def(line)){
            if (!valid_end_macro_def(line, line_number))
                return 0; /* Error found */
            state = OUTSIDE_MACRO_DEF;
            continue;
        }

        if (state == INSIDE_MACRO_DEF) {
            add_line_to_mcro(created_macro, line, line_number);
            continue;
        }

        /* Check if the line contains macro names and deploy the macros to the target file */
        if (check_line_for_macro(line, macro_list)) {
            deploy_macros_in_line(line, target_file, macro_list);
        } else {
            write_line_to_file(line, target_file);
        }
    }

    /* Free dynamically allocated memory */
    free_macro_list(macro_list);

    return 1;
}
