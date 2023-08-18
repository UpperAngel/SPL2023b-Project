#include "../../globals.h"
#include "../../error-handling/errors.h"

void write_opcode(struct InstructionStructure* encoded_inst, enum opcode op) {
    encoded_inst->opcode = op & 0xF; /* Mask the opcode to keep only the 4 least significant bits (LSBs) */
}
void write_source_addressing(struct InstructionStructure* encoded_inst, unsigned int source_addressing) {
    encoded_inst->source_addressing = source_addressing & 0x7; /* Mask the source_addressing to keep only the 3 least significant bits (LSBs) */
}
void write_target_addressing(struct InstructionStructure* encoded_inst, unsigned int target_addressing) {
    encoded_inst->target_addressing = target_addressing & 0x7;
}
void encoding_A_R_E(struct InstructionStructure* encoded_inst, unsigned int encoding_type) {
    encoded_inst->encoding_type = encoding_type & 0x3;
}
void encode_bits_2_to_11(int number, struct InstructionStructure* encoded_inst) {
    /* Get the binary representation of the number as a 32-bit unsigned integer */
    unsigned int binary_rep = (unsigned int)number;

    /* Extract the target_addressing, opcode, and source_addressing bits using bitwise operations */
    encoded_inst->target_addressing = (binary_rep & 0x7); /* Last 3 bits */
    encoded_inst->opcode = ((binary_rep >> 3) & 0xF); /* Next 4 bits */
    encoded_inst->source_addressing = ((binary_rep >> 7) & 0x7); /* First 3 bits */
}
void encode_bits_2_to_6(int number, struct InstructionStructure* encoded_inst) {
    /* Get the binary representation of the number as a 32-bit unsigned integer */
    unsigned int binary_rep = (unsigned int)number;

    /* Extract bits 0, 1, and 2 and store them in bits 0, 1, and 2 of the target_addressing field */
    encoded_inst->target_addressing |= (binary_rep & 0x7); /* Bits 0, 1, and 2 stored in bits 0, 1, and 2 of target_addressing */

    /* Extract bits 3 and 4 and store them in bits 1 and 2 of the opcode field */
    encoded_inst->opcode |= ((binary_rep >> 3) & 0x3) << 1; /* Bits 3 and 4 stored in bits 1 and 2 of opcode */
}
void encode_bits_7_to_11(int number, struct InstructionStructure* encoded_inst) {
    /* Get the binary representation of the number as a 32-bit unsigned integer */
    unsigned int binary_rep = (unsigned int)number;

    /* Extract bits 0 and 1 and store them in bits 2 and 3 of the opcode field */
    encoded_inst->opcode |= ((binary_rep & 0x3) << 2); /* Bits 0 and 1 stored in bits 2 and 3 */

    /* Extract bits 2, 3, and 4 and store them in bits 9, 10, and 11 of the source_addressing field */
    encoded_inst->source_addressing |= ((binary_rep & 0x1C) << 6); /* Bits 2, 3, and 4 stored in bits 9, 10, and 11 */
}
void encode_opcode(struct InstructionStructure* encoded_inst, enum opcode operation) {
    switch (operation) {
        /* First Group */
        case MOV_OP:
            write_opcode(encoded_inst,MOV_OP);
            break;
        case CMP_OP:
            write_opcode(encoded_inst,CMP_OP);
            break;
        case ADD_OP:
            write_opcode(encoded_inst,ADD_OP);
            break;
        case SUB_OP:
            write_opcode(encoded_inst,SUB_OP);
            break;
        case LEA_OP:
            write_opcode(encoded_inst,LEA_OP);
            break;

            /* Second Group */
        case CLR_OP:
            write_opcode(encoded_inst,CLR_OP);
            break;
        case NOT_OP:
            write_opcode(encoded_inst,NOT_OP);
            break;
        case INC_OP:
            write_opcode(encoded_inst,MOV_OP);
            break;
        case DEC_OP:
            write_opcode(encoded_inst,DEC_OP);
            break;
        case JMP_OP:
            write_opcode(encoded_inst,JMP_OP);
            break;
        case BNE_OP:
            write_opcode(encoded_inst,BNE_OP);
            break;
        case JSR_OP:
            write_opcode(encoded_inst,JSR_OP);
            break;
        case RED_OP:
            write_opcode(encoded_inst,MOV_OP);
            break;
        case PRN_OP:
            write_opcode(encoded_inst,PRN_OP);
            break;

            /* Third Group */
        case RTS_OP:
            write_opcode(encoded_inst,RTS_OP);
            break;
        case STOP_OP:
            write_opcode(encoded_inst,STOP_OP);
            break;

            /* Failed/Error */
            /************************************* NEED TO SEE WHAT TO DO WITH THESE ***********************/
        default:
            break;
    }
}
void encode_second_and_third_word(struct InstructionStructure *instruction, enum OperandType OPERAND_TYPE, char *operand, int is_source_operand){
    int number = 0;

    switch (OPERAND_TYPE){
        case NUMBER:
            number = atoi(operand);
            encode_bits_2_to_11(number,instruction);
            break;
        case REGISTER:
            number = get_register_number (operand);
            if (is_source_operand){
                encode_bits_7_to_11(number,instruction);
            }else { /* is target operand */
                encode_bits_2_to_6(number,instruction);
            }
            break;
        default:
            /* do nothing */
            break;
    }
}