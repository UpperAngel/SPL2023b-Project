#include <string.h>

#include "error-handling/errors.h"
#include "first-pass/first-pass-headers/first_pass.h"
#include "globals/globals.h"
#include "pre-processor/pre-processor-headers/pre_processor.h"
#include "second-pass/second_pass.h"

#define COUNTER_INIT 0

int main(int argc, char const *argv[])
{
    int error_found = 0;
    int arg_index;
    int ic, dc;
    char *filename = NULL;
    char *full_filename = NULL;
    FILE *curr_as_file = NULL;
    FILE *curr_am_file = NULL;

    Symbol *symbol_head = NULL;
    struct SymbolNameAndIndex *symbol_name_and_index;
    struct InstructionStructure instruction_array[1024];
    struct DataStructure data_array[1024];

    if (argc < 2)
    {
        printf("usage: %s <filename without '.as' extension>\n", argv[0]);
        exit(1);
    }

    for (arg_index = 1; arg_index < argc; arg_index++)
    {
        filename = my_strdup(argv[arg_index]);
        full_filename = malloc(strlen(filename) + sizeof(char) * 4);
        strcpy(full_filename, filename);
        strcat(full_filename, ".as");

        

        ic = COUNTER_INIT;
        dc = COUNTER_INIT;

        if (strstr(filename, ".as") != NULL)
        {
            printf("usage: %s <filename without '.as' extension>\n", argv[0]);
            exit(1);
        }
        curr_as_file = fopen(full_filename, "r");
        if (curr_as_file == NULL)
        {
            printf("can't open file: %s\n", filename);
            exit(1);
        }
        curr_am_file = create_file(filename, ".am");
        if (curr_am_file == NULL)
        {
            exit(1);
        }

        
        /* pre processor */
        process_file(curr_as_file, curr_am_file);
        fclose(curr_as_file);
        rewind(curr_am_file);

        first_pass(curr_am_file, instruction_array, data_array, &symbol_head, &symbol_name_and_index, &ic, &dc, &error_found);
        fclose(curr_am_file);

        second_pass(filename, symbol_head, instruction_array, data_array, symbol_name_and_index, ic, dc, &error_found);

        free(filename);
        free(full_filename);
        free_symbol_list(symbol_head);
    }

    return 0;
}
