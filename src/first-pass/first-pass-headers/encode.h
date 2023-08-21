#ifndef __ENCODE_H
#define __ENCODE_H

#include "../../error-handling/errors.h"
#include "../../globals/globals.h"

#define BIT4MASK 0xF
#define BIT3MASK 0x7
#define BIT2MASK 0x3
int get_register_number(char* register_str);
void write_opcode(struct InstructionStructure* encoded_inst, enum opcode op);
void write_source_addressing(struct InstructionStructure* encoded_inst, unsigned int source_addressing);
void write_target_addressing(struct InstructionStructure* encoded_inst, unsigned int target_addressing);
void encoding_A_R_E(struct InstructionStructure* encoded_inst, unsigned int encoding_type);
void encode_bits_2_to_11(int number, struct InstructionStructure* encoded_inst);
void encode_bits_2_to_6(int number, struct InstructionStructure* encoded_inst);
void encode_bits_7_to_11(int number, struct InstructionStructure* encoded_inst);
void encode_opcode(struct InstructionStructure* encoded_inst, enum opcode operation);
void encode_second_and_third_word(struct InstructionStructure* instruction, enum OperandType OPERAND_TYPE, char* operand, int is_source_operand);

#endif