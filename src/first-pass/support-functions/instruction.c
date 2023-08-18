#include "../first-pass-headers/instruction.h"

#include "../../error-handling/errors.h"
#include "../../globals/globals.h"

void assign_operands(char words_array[LEN][LEN], char **operand1, char **operand2, int symbol_definition, int number_of_operands) {
    int index = 1; /* Initialize the IC_index variable to 1 */

    if (symbol_definition) /* If symbol_definition is true (non-zero), increment the IC_index by 2 */
        index = index + 2;

    *operand1 = words_array[index]; /* Assign the value of the word_array at the incremented IC_index to operand1.
                                         The prefix increment operator (++IC_index) is used to access the next element in the array. */

    if (number_of_operands == 2) {      /* If the number_of_operands is equal to 2 */
        index = index + 2;              /* Increment the IC_index by 2 skipping the ',' char */
        *operand2 = words_array[index]; /* Assign the value of the word_array at the updated IC_index to operand2 */
    }
}

int valid_instruction(char words_array[LEN][LEN], int line_number, int symbol_definition) {
    int instruction_index = 0;
    Opcode operation_code = NULL_OP;
    char *operand1 = NULL;
    char *operand2 = NULL;
    enum OperandType operand1_type = UNDEFINED;
    enum OperandType operand2_type = UNDEFINED;
    int number_of_operands = 0;
    int is_valid = 1; /* Assume the instruction is valid initially */

    /* Increment the index if there is symbol_definition */
    if (symbol_definition) {
        instruction_index = instruction_index + 2;
    }

    /* assign operands, between the two operands there is ',' char so we skip it */
    operand1 = words_array[instruction_index + 1];
    operand2 = words_array[instruction_index + 3];

    /* check the line for correct comma structure */
    if (!valid_commas_in_instruction(words_array, instruction_index + 1, line_number)) {
        return 0;
    }

    operation_code = get_opcode(words_array[instruction_index]);
    if (operation_code == NULL_OP) {
        handle_error(UnknownOperationName, line_number);
        return 0;
    }

    number_of_operands = get_number_of_operands(operation_code);

    switch (number_of_operands) {
        case 0:
            if (operand1 != NULL || operand2 != NULL) {
                is_valid = 0;
            }
            break;
        case 1:
            if (operand1 == NULL || operand2 != NULL) {
                is_valid = 0;
            }
            break;
        case 2:
            if (operand1 == NULL || operand2 == NULL) {
                is_valid = 0;
            }
            break;
        default:
            /* do nothing */
            break;
    }

    if (!is_valid) {
        handle_error(InvalidNumberOfOperands, line_number);
        return 0; /* Return 0 for an invalid instruction */
    }

    /* Here is a check for excess char at the end of the instruction line */
    if (number_of_operands == 1 || number_of_operands == 0) {
        if (words_array[instruction_index + number_of_operands + 1] != NULL) {
            handle_error(ExcessCharactersInInstruction, line_number);
            return 0;
        }
    } else {
        if (words_array[instruction_index + 4][0] != '\0') {
            handle_error(ExcessCharactersInInstruction, line_number);
            return 0;
        }
    }

    operand1_type = get_operand_type(operand1);
    operand2_type = get_operand_type(operand2);

    switch (number_of_operands) {
        case 0:
            if (operand1_type != UNDEFINED || operand2_type != UNDEFINED) {
                is_valid = 0;
            }
            break;
        case 1:
            if (operand1_type == UNDEFINED || operand2_type != UNDEFINED) {
                is_valid = 0;
            }
            break;
        case 2:
            if (operand1_type == UNDEFINED || operand2_type == UNDEFINED) {
                is_valid = 0;
            }
            break;
        default:
            /* do nothing */
            break;
    }

    if (!is_valid) {
        handle_error(UndefinedOperandsNames, line_number);
        return 0;
    }

    if (!valid_addressing_for_operands(operand1_type, operand2_type, operation_code, number_of_operands)) {
        handle_error(InvalidOperandAddressing, line_number);
        return 0;
    }
    /* passed all the checks */
    return 1;
}

int valid_addressing_for_operands(enum OperandType operand1_type, enum OperandType operand2_type, Opcode operation_code, int number_of_operands) {
    enum OperandType source_operand = UNDEFINED;
    enum OperandType target_operand = UNDEFINED;

    if (number_of_operands == 1)
        target_operand = operand1_type;
    else if (number_of_operands == 2) {
        source_operand = operand1_type;
        target_operand = operand2_type;
    }

    switch (operation_code) {
        case (MOV_OP):
        case (ADD_OP):
        case (SUB_OP):
            if (source_operand == UNDEFINED || target_operand == NUMBER || target_operand == UNDEFINED)
                return 0;
            break;

        case (CMP_OP):
            if (source_operand == UNDEFINED || target_operand == UNDEFINED)
                return 0;
            break;

        case (LEA_OP):
            if (source_operand != SYMBOL || target_operand == UNDEFINED || target_operand == NUMBER)
                return 0;
            break;

        case (NOT_OP):
        case (CLR_OP):
        case (INC_OP):
        case (DEC_OP):
        case (JMP_OP):
        case (BNE_OP):
        case (RED_OP):
        case (JSR_OP):
            if (source_operand != UNDEFINED || target_operand == UNDEFINED || target_operand == NUMBER)
                return 0;
            break;

        case (PRN_OP):
            if (source_operand != UNDEFINED || target_operand == UNDEFINED)
                return 0;
            break;

        case (RTS_OP):
        case (STOP_OP):
            if (source_operand != UNDEFINED || target_operand != UNDEFINED)
                return 0;
            break;
    }

    /* correct operands addressing */
    return 1;
}

int handle_valid_instruction(char words_array[LEN][LEN], struct InstructionStructure *instructions_array, int IC, int symbol_definition, int line_number, struct SymbolNameAndIndex **pass2_list_head) {
    enum OperandType operand1_type = UNDEFINED;
    enum OperandType operand2_type = UNDEFINED;
    int index = 0;
    int number_of_operands = 0;
    enum opcode op;
    char *operand1 = NULL;
    char *operand2 = NULL;

    /* Increment the index if symbol_definition is true */
    if (symbol_definition) {
        index = index + 2;
    }

    /* Translate instruction name to opcode */
    op = get_opcode(words_array[index]);

    /* Get the number of operands for the instruction */
    number_of_operands = get_number_of_operands(op);

    /* Assign operands if there are any */
    if (number_of_operands) {
        assign_operands(words_array, &operand1, &operand2, symbol_definition, number_of_operands);
        operand1_type = get_operand_type(operand1);
        operand2_type = get_operand_type(operand2);
    }

    /* Encode opcode of the first word*/
    encode_opcode(&instructions_array[IC], op);

    /* first word of instruction A,R,E is always 0 */
    encoding_A_R_E(&instructions_array[IC], 0);

    switch (operand1_type) {
        case UNDEFINED:
            write_source_addressing(&instructions_array[IC], 0);
            write_target_addressing(&instructions_array[IC], 0);
            return 1; /* one line of instructions was added */
        case NUMBER:
            if (number_of_operands == 1) {
                write_source_addressing(&instructions_array[IC], 0);
                write_target_addressing(&instructions_array[IC], 1);
                encode_second_and_third_word(&instructions_array[IC + 1], NUMBER, operand1, 0);
                return 2; /* two lines of instructions were added */
            } else {      /* number of operands is 2 */
                write_source_addressing(&instructions_array[IC], 1);
                /* A,R,E of second word is 0 */
                encoding_A_R_E(&instructions_array[IC + 1], 0);
                encode_second_and_third_word(&instructions_array[IC + 1], NUMBER, operand1, number_of_operands);
            }
            break;
        case REGISTER:
            /* A,R,E of second word is 0 */
            encoding_A_R_E(&instructions_array[IC + 1], 0);

            if (number_of_operands == 1) {
                write_target_addressing(&instructions_array[IC], 5);
                encode_second_and_third_word(&instructions_array[IC + 1], REGISTER, operand1, number_of_operands);
                return 2;
            } else { /* number of operands is 2 */
                write_source_addressing(&instructions_array[IC], 5);
                encode_second_and_third_word(&instructions_array[IC + 1], REGISTER, operand1, 1);
            }
            break;
        case SYMBOL: /* encoding of extra symbol words happens in the second pass */
            insertNode(pass2_list_head, createNode(operand1, IC + 1, line_number));
            if (number_of_operands == 1) {
                write_target_addressing(&instructions_array[IC], 3);
                return 2;
            }
            write_source_addressing(&instructions_array[IC], 3);
            break;
    }
    /* handling second operand */
    switch (operand2_type) {
        case NUMBER:
            write_target_addressing(&instructions_array[IC], 1);
            encode_second_and_third_word(&instructions_array[IC + 2], NUMBER, operand2, number_of_operands);
            return 3;
        case REGISTER:
            write_target_addressing(&instructions_array[IC], 5);
            if (operand1_type == REGISTER) {
                encode_second_and_third_word(&instructions_array[IC + 1], REGISTER, operand2, 0);
                return 2;
            }
            encode_second_and_third_word(&instructions_array[IC + 2], REGISTER, operand2, 0);
            return 3;
        case SYMBOL:
            write_target_addressing(&instructions_array[IC], 3);
            insertNode(pass2_list_head, createNode(operand2, IC + 2, line_number));
            return 3;
    }
    return 0; /* random value change laterrrrrrrrrrrrrrrr */
}

int valid_commas_in_instruction(char words_array[LEN][LEN], int starting_index, int line_number) {
    int i = starting_index;

    /* Check if the first word is a comma */
    if (words_array[i][0] == ',') {
        handle_error(CommaAtStart, line_number); /* Comma at start */
        return 0;                                /* Invalid */
    }

    /* if next word is the end of the line then its valid instruction */
    if (words_array[++i][0] == '\0')
        return 1;
    else if (words_array[i][0] != ',') {
        handle_error(MissingComma, line_number);
        return 0;
    }

    if (words_array[++i][0] == ',') {
        handle_error(ConsecutiveCommas, line_number);
        return 0;
    } else if (words_array[i][0] == '\0') {
        handle_error(EndsWithComma, line_number);
        return 0;
    }
    if (words_array[++i][0] == ',') {
        handle_error(EndsWithComma, line_number);
        return 0;
    }

    return 1; /* Valid */
}

int get_number_of_operands(Opcode cmd_opcode) {
    if (cmd_opcode > NULL_OP) {
        if (cmd_opcode < 5) {
            return 2;

        } else if (cmd_opcode < 14) {
            return 1;

        } else {
            if (cmd_opcode <= 15) {
                return 0;
            }
            return -1;
        }
    }
    return -1;
}
enum OperandType get_operand_type(char *operand) {
    if (is_number(operand)) {
        return NUMBER;
    } else if (is_register(operand)) {
        return REGISTER;
    } else if (is_symbol(operand))
        return SYMBOL;
    else
        return UNDEFINED;
}
