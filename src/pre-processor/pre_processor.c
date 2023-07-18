#include "../include/pre_processor.h"
void process_file(FILE *file, FILE *target_file)
{
    int is_macro = 0;
    char line[MAXLEN];
    char *curr_mcro_name = NULL; // Initialize to NULL
    mcro *created_macro;
    mcro *curr_macro;
    struct macroList *macro_list = createMacroList();

    while (fgets(line, MAXLEN, file) != NULL)
    {
        /* Remove newline character at the end of the line */
        if (line[strlen(line) - 1] == '\n')
            line[strlen(line) - 1] = '\0';

        if (is_valid_macro_def(line))
        {
            is_macro = 1;
            curr_mcro_name = strtok(line, " "); /* Call strtok with 'line' as the first argument */
            created_macro = create_mcro(curr_mcro_name);
            add_to_macro_table(created_macro, &(macro_list));
        }

        if (is_endmcro(line))
        {
            is_macro = 0;
            delete_line(line);
        }

        if (is_comment(line))
        {
            delete_line(line);
        }

        if (is_empty(line))
        {
            delete_line(line);
        }

        if (is_macro == 1)
        {
            add_line_to_mcro(created_macro, line);
        }

        if ((curr_macro = find_macro_by_name(macro_list, line)))
        {
            deploy_macro(curr_macro, target_file);
        }
    }

    /*  Free the dynamically allocated memory before exiting the function */
    free_macro_list(macro_list);
}
