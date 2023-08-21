#include <string.h>

#include "error-handling/errors.h"
#include "first-pass/first-pass-headers/first_pass.h"
#include "globals/globals.h"
#include "pre-processor/pre-processor-headers/pre_processor.h"
#include "second-pass/second_pass.h"

#define COUNTER_INIT 0

int main(int argc, char const *argv[]) {
    int error_found = 0;
    int arg_index;
    int ic, dc;
    char *curr_filename = NULL;
    FILE *curr_as_file = NULL;
    FILE *curr_am_file = NULL;

    Symbol *curr_symbol;
    struct InstructionStructure instruction_array[1024];
    struct DataStructure data_array[1024];
    struct SymbolNameAndIndex *symbol_name_and_index;

    if (argc < 2) {
        printf("usage: %s <filename without \"as\" extension>\n", argv[0]);
        exit(1);
    }
    for (arg_index = 1; arg_index < argc; arg_index++) {
        ic = COUNTER_INIT;
        dc = 0;
        curr_filename = my_strdup(argv[arg_index]);

        curr_as_file = fopen(strcat(curr_filename, ".as"), "r");
        curr_am_file = create_file(curr_filename, ".am");
        /* pre processor */
        process_file(curr_as_file, curr_am_file);
        fclose(curr_as_file);
        rewind(curr_am_file);

       
        first_pass(curr_am_file, instruction_array, data_array, &curr_symbol, &symbol_name_and_index, &ic, &dc, &error_found);
        second_pass(curr_filename, curr_symbol, instruction_array, data_array,symbol_name_and_index, ic, dc, &error_found); 

        fclose(curr_am_file);
        free(curr_filename);
        free(curr_symbol);
    }
    return 0;
}
