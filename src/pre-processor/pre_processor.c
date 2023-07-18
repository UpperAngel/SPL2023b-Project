#include "../include/pre_processor.h"

void process_file(FILE *source_file, FILE *target_file) {
    char line[MAXLEN];
    char *curr_mcro_name = NULL; /* Initialize to NULL */
    mcro *created_macro;
    struct macroList *macro_list = createMacroList();

    while (fgets(line, MAXLEN, source_file) != NULL) {
        /* Remove newline character at the end of the line */
        if (line[strlen(line) - 1] == '\n')
            line[strlen(line) - 1] = '\0';

        /* Check if the line is a valid macro definition */
        if (is_valid_macro_def(line)) {
            /* If the line is a valid macro definition, process it and continue to the next line. */
            curr_mcro_name = strtok(line, " "); /* Call strtok with 'line' as the first argument */
            created_macro = create_mcro(curr_mcro_name);
            add_to_macro_table(created_macro, &(macro_list));
            continue;
        }

        /* Check if the line is an endmcro directive */
        if (is_endmcro(line)) {
            /* If the line is an endmcro directive, skip it and continue to the next line. */
            continue;
        }

        /* Check if the line is a comment */
        if (is_comment(line)) {
            /* If the line is a comment, skip it and continue to the next line. */
            continue;
        }

        /* Check if the line is empty */
        if (is_empty(line)) {
            /* If the line is empty, skip it and continue to the next line. */
            continue;
        }

        /* Write the line to the target file if it should not be deleted */
        write_line_to_file(line, target_file);
    }

    /* Free the dynamically allocated memory before exiting the function */
    free_macro_list(macro_list);
}