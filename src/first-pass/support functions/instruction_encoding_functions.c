#include "/home/david/CLionProjects/My-project/Project main branch/src/FIRST ---PASS/header files/first_pass_headers.h"
void assign_operands(char words_array[LEN][LEN], char **operand1, char **operand2, int symbol_definition, int number_of_operands){
    int index = 0;  /* Initialize the index variable to 0 */

    if (symbol_definition)  /* If symbol_definition is true (non-zero), increment the index by 1 */
        index++;

    *operand1 = words_array[++index];  /* Assign the value of the word_array at the incremented index to operand1.
                                          The prefix increment operator (++index) is used to access the next element in the array. */

    if (number_of_operands == 2){  /* If the number_of_operands is equal to 2 */
        index = index + 2;  /* Increment the index by 2 skipping the ',' char */
        *operand2 = words_array[index];  /* Assign the value of the word_array at the updated index to operand2 */
    }
}
enum opcode translate_instruction_name(const char* instruction_name_str) {
    int i = 0;
    while (commands[i].instruction_name != NULL) {
        if (strcmp(commands[i].instruction_name, instruction_name_str) == 0) {
            return commands[i].opcode_value;
        }
        i++;
    }
    return NULL_OP; /* If not found, return NULL_OP */
}


/* ENCODE FUNCTIONS */
void write_opcode(struct InstructionStructure* encoded_inst, enum opcode op) {
    encoded_inst->opcode = op & 0xF; /* Mask the opcode to keep only the 4 least significant bits (LSBs) */
}
void write_source_addressing(struct InstructionStructure* encoded_inst, unsigned int source_addressing) {
    encoded_inst->source_addressing = source_addressing & 0x7; /* Mask the source_addressing to keep only the 3 least significant bits (LSBs) */
}
void write_target_addressing(struct InstructionStructure* encoded_inst, unsigned int target_addressing) {
    encoded_inst->target_addressing = target_addressing & 0x7;
}
void write_encoding_type(struct InstructionStructure* encoded_inst, unsigned int encoding_type) {
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
    }
}

/* IS A functions */
int is_symbol(const char *word)
{
    char *dots = NULL;

    dots = strchr(word, ':');
    if (dots != NULL)
    {
        return 1;
    }

    return 0;
}
int is_register(const char *operand) {
    if (operand[0] == '@' && operand[1] == 'r' && isdigit(operand[2])) {
        if ((operand[2] - '0') <= 7) {
            return 1;
        }
    }
    return 0;
}
int is_number(const char *str) {
    if (*str == '\0') {
        return 0;
    }

    // Check for a plus (+) or minus (-) sign at the beginning of the string
    if (*str == '+' || *str == '-') {
        str++; // Move to the next character
    }

    while (*str) {
        if (!isdigit(*str)) {
            return 0;
        }
        str++;
    }
    return 1;
}


/* get functions */
int get_register_number(char *register_str) {
    int register_number;
    if (register_str == NULL || strlen(register_str) != 3) {
        /* Invalid input: register_str is NULL or not of length 3 */
        /* You can handle this case as needed (e.g., return an error code) */
        return -1; /* Return a default value or an error code */
    }

    if (register_str[0] != '@' || register_str[1] != 'r') {
        /* Invalid input: register_str does not start with '@r' */
        /* You can handle this case as needed (e.g., return an error code) */
        return -1; /* Return a default value or an error code */
    }

    /* Extract the register number from the string and convert it to an integer */
     register_number = register_str[2] - '0';

    /* Check if the register number is valid (0 to 7) */
    if (register_number < 0 || register_number > 7) {
        /* Invalid input: register number is out of range */
        /* You can handle this case as needed (e.g., return an error code) */
        return -1; /* Return a default value or an error code */
    }

    return register_number;
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
Opcode get_opcode(char *command) {
    int i;

    if (command == NULL) {
        return NULL_OP;
    }

    for (i = 0; commands[i].instruction_name != NULL; i++) {
        if (strcmp(command, commands[i].instruction_name) == 0) {
            return commands[i].opcode_value;
        }
    }

    return NULL_OP;
}
enum OperandType get_operand_type(char* operand) {
     if (is_number(operand)) {
        return NUMBER;
    } else if (is_register(operand)) {
        return REGISTER;
    } else if (is_symbol(operand))
        return SYMBOL;
     else
         return NONE; /* default operand type or error type */;
}



int handle_valid_instruction(char words_array[LEN][LEN], struct InstructionStructure *instructions_array, int IC, int symbol_definition) {
    enum OperandType operand1_type = NONE;
    enum OperandType operand2_type = NONE;
    int index = 0;
    int number_of_operands = 0;
    enum opcode op;
    char *operand1 = NULL;
    char *operand2 = NULL;

    /* Increment the index if symbol_definition is true */
    if (symbol_definition) {
        index++;
    }

    /* Translate instruction name to opcode */
    op = translate_instruction_name(words_array[index]);

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
    write_encoding_type(&instructions_array[IC], 0);

    switch (operand1_type) {
        case NONE:
            write_source_addressing(&instructions_array[IC], 0);
            write_target_addressing(&instructions_array[IC], 0);
            return 1; /* one line of instructions was added */
        case NUMBER:
            if (number_of_operands == 1) {
                write_source_addressing(&instructions_array[IC], 0);
                write_target_addressing(&instructions_array[IC], 1);
                encode_second_and_third_word(&instructions_array[IC + 1], NUMBER, operand1,0);
                return 2; /* two lines of instructions were added */
            } else { /* number of operands is 2 */
                write_source_addressing(&instructions_array[IC], 1);
                /* A,R,E of second word is 0 */
                write_encoding_type(&instructions_array[IC + 1], 0);
                encode_second_and_third_word(&instructions_array[IC + 1], NUMBER, operand1,number_of_operands);
            }
            break;
        case REGISTER:
            /* A,R,E of second word is 0 */
            write_encoding_type(&instructions_array[IC + 1], 0);

            if (number_of_operands == 1) {
                write_target_addressing(&instructions_array[IC], 5);
                encode_second_and_third_word(&instructions_array[IC + 1], REGISTER, operand1,number_of_operands);
                return 2;
            } else { /* number of operands is 2 */
                write_source_addressing(&instructions_array[IC], 5);
                encode_second_and_third_word(&instructions_array[IC + 1], REGISTER, operand1,1);
            }
            break;
        case SYMBOL: /* encoding of extra symbol words happens in the second pass */
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
            encode_second_and_third_word(&instructions_array[IC + 2], NUMBER, operand2,number_of_operands);
            return 3;
        case REGISTER:
            write_target_addressing(&instructions_array[IC], 5);
            if (operand1_type == REGISTER) {
                encode_second_and_third_word(&instructions_array[IC + 1], REGISTER, operand2,0);
                return 2;
            }
            encode_second_and_third_word(&instructions_array[IC + 2], REGISTER, operand2,0);
            return 3;
        case SYMBOL:
            write_target_addressing(&instructions_array[IC], 3);
            return 3;
    }
    return 0; /* random value change laterrrrrrrrrrrrrrrr */
}


