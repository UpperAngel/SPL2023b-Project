#include "../first-pass/first-pass-headers/symbol_table.h"
#include "../pre-processor/pre-processor header files/helper_fucntions.h"
#include "data-and-instructions.h"
#include "symbolNameAndIndex.h"

/*
gets name of label, index.
lookup name -> go to index -> encode
*/

void write_encoding_type(struct InstructionStructure *encoded_inst, unsigned int encoding_type);
void encode_bits_2_to_11(int number, struct InstructionStructure *encoded_inst);

void second_pass(const char *file_name, SymbolTable *symbol_table, struct InstructionStructure **instruction_array, struct DataStructure *data_array, struct SymbolNameAndIndex *symbol_name_and_index, int *error_ptr) {
    struct SymbolNameAndIndex *curr_missing = symbol_name_and_index;

    int i;
    FILE *ob_file;
    FILE *ent_file;
    FILE *ext_file;
    Symbol *curr_symbol;

    if (*error_ptr = 1) {
        return;
    }
    while (curr_missing != NULL) {
        Symbol *symbol = get_symbol_by_name(symbol_table, curr_missing->name);

        if (symbol != NULL) {
            if (symbol->type == EXTERN) {
                write_encoding_type(instruction_array[curr_missing->IC_index], 0x3);
            } else {
                write_encoding_type(instruction_array[curr_missing->IC_index], 0x1);
            }
            encode_bits_2_to_11(symbol->val, instruction_array[curr_missing->IC_index]);

        } else {
            /* there is no symbol with that name. */
            *error_ptr = 1; /* found error */
        }

        curr_missing = curr_missing->next;
    }

    if (*error_ptr = 1) {
        return;
    }

    /* files stuff */
    ob_file = create_file(file_name, ".ob");
    ent_file = create_file(file_name, ".ent");
    ext_file = create_file(file_name, ".ext");

    for (i = 0; i < symbol_table->length; i++) {
        
        curr_symbol = symbol_table->symbols[i];
        if (curr_symbol == ENTRY) {
            fprintf(ent_file, "%s: %d\n", curr_symbol->name, curr_symbol->val);
        }
    }

    /* free in main*/
}

void write_encoding_type(struct InstructionStructure *encoded_inst, unsigned int encoding_type) {
    encoded_inst->encoding_type = encoding_type & 0x3;
}
void encode_bits_2_to_11(int number, struct InstructionStructure *encoded_inst) {
    /* Get the binary representation of the number as a 32-bit unsigned integer */
    unsigned int binary_rep = (unsigned int)number;

    /* Extract the target_addressing, opcode, and source_addressing bits using
     * bitwise operations */
    encoded_inst->target_addressing = (binary_rep & 0x7);        /* Last 3 bits */
    encoded_inst->opcode = ((binary_rep >> 3) & 0xF);            /* Next 4 bits */
    encoded_inst->source_addressing = ((binary_rep >> 7) & 0x7); /* First 3 bits */
}
