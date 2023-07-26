#include "pre-processor header files/pre_processor.h"
#include "handle_error.h"

/* Function to process the source file and generate the target file */
int process_file(FILE* source_file, FILE* target_file) {

    enum MacroState state = OUTSIDE_MACRO_DEF; /* Initialize the state of the macro processing to be outside a macro definition */
    char line[MAXLEN]; /* Buffer to store each line read from the source file */
    char curr_macro_name[MACRO_NAME_LENGTH]; /* Buffer to store the name of the current macro being processed */
    mcro* created_macro = NULL; /* Pointer to the macro structure representing the macro being created */
    struct macroList* macro_list = createMacroList(); /* Pointer to the macro list containing all defined macros */
    int end_macro_check; /* Variable to store the result of the check for the end of a macro definition */
    int valid_macro_check; /* Variable to store the result of the check for the start of a macro definition */
    int line_number = 0; /* Variable to keep track of the line number being processed in the source file */

    while (fgets(line, MAXLEN, source_file) != NULL) {
        line_number++; /* Increment the number of the line */

        /* Remove newline character at the end of a string */
        remove_newline(line);

       /* Check if the current line is a comment or an empty line if so skip it */
        if(comment_or_empty(line))
            continue;

        /* Check if the line marks the end of a macro definition */
        end_macro_check = valid_end_macro_def(line);
        if (end_macro_check == END_MACRO_FOUND) {
            state = OUTSIDE_MACRO_DEF;
            continue;
        } else if (end_macro_check == END_MACRO_WITH_OTHER_CHARACTERS) {
            /* Handle error: Excess characters at the end of macro definition */
            handle_error(ExcessCharactersAtEndMacro, line_number);
            return 0;
        }

        /* Check if the line is the start of a valid macro definition */
        valid_macro_check = valid_start_macro_def(line);
        if (state == OUTSIDE_MACRO_DEF && valid_macro_check == 1) {
            state = INSIDE_MACRO_DEF;
            strcpy(curr_macro_name, get_second_token(line));
            created_macro = create_mcro(curr_macro_name);
            add_to_macro_table(created_macro, &macro_list);
            continue;
        } else if (valid_macro_check == 0) {
            /* Handle error: Excess characters at the start of macro definition */
            handle_error(ExcessCharactersAtStartMacro, line_number);
            return 0;
        }

        if (state == INSIDE_MACRO_DEF) {
            add_line_to_mcro(created_macro, line);
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
