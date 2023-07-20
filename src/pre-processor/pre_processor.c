#include "../include/pre_processor.h"

/* Function to process the source file and extract macros */
int process_file(FILE *source_file, FILE *target_file) {
    enum State state = OUTSIDE_MACRO_DEF;
    char line[MAXLEN];
    char curr_mcro_name[MAXLEN]; /* Initialize to an array with MAXLEN size */
    mcro *created_macro = NULL;
    struct macroList *macro_list = createMacroList();

    /* Loop through each line in the source file */
    while ( fgets(line, MAXLEN, source_file) != NULL ) {
        /* Remove newline character at the end of the line */
        remove_newline(line);

        /* Check if the line is a comment or an empty line, then skip it */
        if (skip_line(line))
            continue;

        /* Check if the line marks the end of a macro definition */
        if (valid_end_macro(line) == END_MACRO_FOUND) {
            state = OUTSIDE_MACRO_DEF;
            continue;
        }
        else if (valid_end_macro(line) == END_MACRO_WITH_OTHER_CHARACTERS)
            return 0; /* Invalid end of macro definition means error so don't make target file (return 0) */


        /* Check if the line is the start of a valid macro definition */
        if (state == OUTSIDE_MACRO_DEF && is_valid_macro_def(line) == 1 ) {
            /* If the line is a valid macro definition, process it and continue to the next line. */
            state = INSIDE_MACRO_DEF;
            created_macro = create_mcro(curr_mcro_name);
            add_to_macro_table(created_macro, &(macro_list));
            continue;
        }

        /* If inside a macro definition, add the line to the current macro */
        if (state == INSIDE_MACRO_DEF) {
            add_line_to_mcro(created_macro, line);
            continue;
        }

        /* Check if the line contains macro names and deploy the macros to the target file */
        if (check_line_for_macro(line, macro_list)) {
            deploy_macros_in_line(line, target_file, macro_list);
            continue;
        }

        /* Write the line to the target file if it should not be deleted */
        write_line_to_file(line, target_file);
    }

    /* Free the dynamically allocated memory before exiting the function */
    free_macro_list(macro_list);
    return 1; /* indicate that the target file was built successfully */
}
