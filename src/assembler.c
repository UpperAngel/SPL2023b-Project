#include "error.h"
#include "first-pass-headers/first_pass_headers.h"
#include "pre-processor/pre-processor-headers/pre_processor.h"
#include "second-pass/second_pass.h"

int main(int argc, char const *argv[]) {
    int error_found = 0;
    int arg_index;
    char *curr_filename;
    Symbol *curr_symbol;
    struct InstructionStructure *instruction_arry[1024];
    struct DataStructure *data_array[1024];
    struct SymbolNameAndIndex *symbol_name_and_index;

    if (arg_index < 2) {
        exit;
    }
    for (arg_index = 1; arg_index < argc; arg_index++) {
        curr_filename = argv[arg_index];

        // call first_pass;
        second_pass(curr_filename, curr_symbol,&instruction_arry,&data_array,symbol_name_and_index, &error_found);
    }
}
