#include "pre-processor/pre-processor header files/pre_processor.h"
#include "first-pass/first-pass-headers/first_pass.h"
#include "second-pass/symbol_list.h"
#include "pre-processor/pre-processor header files/helper_fucntions.h"
int main(int argc, char const *argv[])
{
    FILE *am_file = NULL;
    int arg_index;
    char *curr_filename;
    Symbol *symbol = create_symbol_table(10);
    if (argc < 2) {
        fprintf(stderr, "missing ");
    }
    
    for (arg_index = 1; arg_index < argc; arg_index++)
    {
        curr_filename = argv[arg_index];
        process_file(fopen(curr_filename, "r"), am_file = create_file(curr_filename, ".am"));
    }
    
    return 0;
}

