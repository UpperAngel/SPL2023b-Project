#include <string.h>
#include "error.h"
#include "first-pass/first-pass-headers/first_pass.h"
#include "pre-processor/pre-processor-headers/pre_processor.h"
#include "second-pass/second_pass.h"
#include "pre-processor/pre-processor-headers/helper_fucntions.h"
int main(int argc, char const *argv[]) {
    int error_found = 0;
    int arg_index;
    
    char *curr_filename;
    FILE *curr_as_file;
    FILE *curr_am_file;
    
    
    Symbol *curr_symbol;
    struct InstructionStructure *instruction_array[1024];
    struct DataStructure *data_array[1024];
    struct SymbolNameAndIndex *symbol_name_and_index;

    if (arg_index < 2) {
        exit(1);
    }
    for (arg_index = 1; arg_index < argc; arg_index++) {
        curr_filename = strcat(argv[arg_index], ".as");
        
        curr_as_file = fopen(curr_filename, "r");
        curr_am_file = create_file(curr_filename, ".am");
        process_file(curr_as_file, curr_am_file);

        first_pass(curr_am_file, instruction_array, data_array, &curr_symbol, &symbol_name_and_index);
        
        second_pass(curr_filename, curr_symbol,&instruction_array,&data_array,symbol_name_and_index, &error_found);
    }
    return 0;
}
