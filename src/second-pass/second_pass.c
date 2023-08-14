#include "../first-pass/first-pass-headers/symbol_table.h"
#include "data-and-instructions.h"
#include "symbolNameAndIndex.h"
/*
gets name of label, index.

lookup name -> go to index -> encode

*/
void write_encoding_type(struct InstructionStructure *encoded_inst,
                         unsigned int encoding_type);
void encode_bits_2_to_11(int number, struct InstructionStructure *encoded_inst);

void second_pass(SymbolTable *symbol_table,
                 struct InstructionStructure **instruction_array,
                 struct DataStructure *data_array,
                 struct SymbolNameAndIndex *symbol_name_and_index) {

  struct SymbolNameAndIndex *curr_missing = symbol_name_and_index;

  while (curr_missing != NULL) {
    Symbol *symbol = get_symbol_by_name(symbol_table, curr_missing->name);

    if (symbol != NULL) {
      if (symbol->type == EXTERN) {
        write_encoding_type(instruction_array[curr_missing->IC_index], 0x3);
      } else {
        write_encoding_type(instruction_array[curr_missing->IC_index], 0x1);
      }
      encode_bits_2_to_11(symbol->val,
                          instruction_array[curr_missing->IC_index]);

    } else {
      /* there is no symbol with that name. */
    }

    curr_missing = curr_missing->next;
  }
  /* free in main*/
}

void write_encoding_type(struct InstructionStructure *encoded_inst,
                         unsigned int encoding_type) {
  encoded_inst->encoding_type = encoding_type & 0x3;
}
void encode_bits_2_to_11(int number,
                         struct InstructionStructure *encoded_inst) {
  /* Get the binary representation of the number as a 32-bit unsigned integer */
  unsigned int binary_rep = (unsigned int)number;

  /* Extract the target_addressing, opcode, and source_addressing bits using
   * bitwise operations */
  encoded_inst->target_addressing = (binary_rep & 0x7); /* Last 3 bits */
  encoded_inst->opcode = ((binary_rep >> 3) & 0xF);     /* Next 4 bits */
  encoded_inst->source_addressing =
      ((binary_rep >> 7) & 0x7); /* First 3 bits */
}
