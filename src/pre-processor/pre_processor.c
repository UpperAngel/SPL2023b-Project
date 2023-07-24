#include "pre-processor header files/pre_processor.h"

int process_file(FILE* source_file, FILE* target_file) {
    enum MacroState state = OUTSIDE_MACRO_DEF;
    char line[MAXLEN];
    char curr_mcro_name[MAXLEN];
    mcro* created_macro = NULL;
    struct macroList* macro_list = createMacroList();

    while (fgets(line, MAXLEN, source_file) != NULL) {
        int end_macro_check;
        int valid_macro_check;

       /* remove newline character at the end of a string */
        remove_newline(line);

        /* Check if the line is a comment or an empty line, then skip it */
        if (comment_or_empty(line)) {
            continue;
        }

         end_macro_check = valid_end_macro(line);
        if (end_macro_check == END_MACRO_FOUND) {
            state = OUTSIDE_MACRO_DEF;
            continue;
        } else if (end_macro_check == END_MACRO_WITH_OTHER_CHARACTERS) {
            /* Invalid end of macro definition means an error, so don't make the target file */
            free_macro_list(macro_list);
            return 0;
        }

         valid_macro_check = is_valid_macro_def(line);
        if (state == OUTSIDE_MACRO_DEF && valid_macro_check == 1) {
            state = INSIDE_MACRO_DEF;
            strcpy(curr_mcro_name, get_second_token(line));
            created_macro = create_mcro(curr_mcro_name);
            add_to_macro_table(created_macro, &macro_list);
            continue;
        } else if (valid_macro_check == 0) {
            /* Invalid macro definition means an error, so don't make the target file */
            free_macro_list(macro_list);
            return 0;
        }

        if (state == INSIDE_MACRO_DEF) {
            add_line_to_mcro(created_macro, line);
            continue;
        }

        if (check_line_for_macro(line, macro_list)) {
            deploy_macros_in_line(line, target_file, macro_list);
        } else {
            write_line_to_file(line, target_file);
        }
    }

    free_macro_list(macro_list);
    return 1; /* Indicate that the target file was built successfully */
}

