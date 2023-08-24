#ifndef __INSTRUCTION_H
#define __INSTRUCTION_H
#include <stdlib.h>
#include <string.h>

#include "../../error-handling/errors.h"
#include "../../globals/globals.h"
#include "../first-pass-headers/encode.h"
#include "../first-pass-headers/instruction.h"

enum opcode get_opcode(const char *instruction_name_str);
int is_register(const char *operand);
int is_number(const char *str);
int is_symbol(const char *symbol);
int handle_valid_instruction(char words_array[LEN][LEN], struct InstructionStructure *instructions_array, int IC, int symbol_definition, int line_number, struct SymbolNameAndIndex **pass2_list_head);
int valid_instruction(char words_array[LEN][LEN], int line_number, int symbol_definition);
int valid_addressing_for_operands(enum OperandType operand1_type, enum OperandType operand2_type, Opcode operation_code, int number_of_operands);
int valid_commas_in_instruction(char words_array[LEN][LEN], int starting_index, int line_number);
int pack_instruction(struct InstructionStructure instruction);
int get_number_of_operands(Opcode cmd_opcode);
enum OperandType get_operand_type(char *operand);
void assign_operands(char words_array[LEN][LEN], char **operand1, char **operand2, int symbol_definition, int number_of_operands);
#endif