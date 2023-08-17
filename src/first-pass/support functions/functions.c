#include "../../Error handling/errors.h"
#include "../first-pass-headers/first_pass_headers.h"

void assign_operands(char words_array[LEN][LEN], char **operand1, char **operand2, int symbol_definition, int number_of_operands){
    int index = 1;  /* Initialize the IC_index variable to 1 */

    if (symbol_definition)  /* If symbol_definition is true (non-zero), increment the IC_index by 2 */
        index= index + 2;

    *operand1 = words_array[index];  /* Assign the value of the word_array at the incremented IC_index to operand1.
                                          The prefix increment operator (++IC_index) is used to access the next element in the array. */

    if (number_of_operands == 2){  /* If the number_of_operands is equal to 2 */
        index = index + 2;  /* Increment the IC_index by 2 skipping the ',' char */
        *operand2 = words_array[index];  /* Assign the value of the word_array at the updated IC_index to operand2 */
    }
}
enum opcode get_opcode(const char* instruction_name_str) {
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

/* IS A functions */
int is_register(const char *operand) {
    if (operand == NULL) {
        return 0; /* Return 0 if the operand pointer is NULL */
    }

    if (operand[0] == '\0') {
        return 0; /* Return 0 if the operand string is empty */
    }

    if (operand[0] == '@' && operand[1] == 'r' && isdigit(operand[2])) {
        if ((operand[2] - '0') <= 7) {
            return 1; /* Return 1 if the operand represents a valid register */
        }
    }
    return 0; /* Return 0 if the operand does not match the register format */
}
int is_number(const char *str) {
    if (str == NULL || *str == '\0') {
        return 0; /* Return 0 if the input pointer is NULL or if the string is empty */
    }

    /* Check for a plus (+) or minus (-) sign at the beginning of the string */
    if (*str == '+' || *str == '-') {
        str++; /* Move to the next character */
    }

    while (*str) {
        if (!isdigit(*str)) {
            return 0; /* Return 0 if a non-digit character is encountered */
        }
        str++; /* Move to the next character */
    }

    return 1; /* Return 1 if all characters are digits */
}
int is_symbol(const char *symbol) {
    size_t i = 1;

    if (is_reserved_keyword(symbol))
        return 0;

    if (symbol == NULL || strlen(symbol) > 31 || strlen(symbol) == 0) {
        return 0; /* Invalid if NULL, longer than 31 chars, or empty */
    }

    if (!isalpha(symbol[0])) {
        return 0; /* Symbol must start with a letter */
    }

    for ( i; symbol[i] != '\0'; i++) {
        if ( !isalnum(symbol[i]) ){
            return 0; /* Invalid character in the symbol */
        }
    }

    return 1; /* The string satisfies the criteria for a valid symbol */
}
int is_reserved_keyword(const char *word) {
    int i = 0;
    for ( i = 0; i < sizeof(reservedKeywords) / sizeof(reservedKeywords[0]); i++) {
        if (strcmp(word, reservedKeywords[i]) == 0) {
            return 1; /* Found a match, it's a reserved keyword */
        }
    }
    return 0; /* No match, it's not a reserved keyword */
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
enum OperandType get_operand_type(char* operand) {
    if ( is_number(operand) ) {
        return NUMBER;
    } else if ( is_register(operand) ) {
        return REGISTER;
    } else if ( is_symbol(operand) )
        return SYMBOL;
    else
        return UNDEFINED;
}


/* INSTRUCTION FUNCTIOS */
int valid_commas_in_instruction(char words_array[LEN][LEN], int starting_index, int line_number) {
    int i = starting_index;

    /* Check if the first word is a comma */
    if (words_array[i][0] == ',') {
        handle_error(CommaAtStart,line_number);  /* Comma at start */
        return 0;  /* Invalid */
    }

    /* if next word is the end of the line then its valid instruction */
    if (words_array[++i][0] == '\0')
        return 1;
    else if (words_array[i][0] != ',') {
        handle_error(MissingComma,line_number);
        return 0;
    }

    if (words_array[++i][0] == ',') {
        handle_error(ConsecutiveCommas,line_number);
        return 0;
    } else if (words_array[i][0] == '\0'){
        handle_error(EndsWithComma,line_number);
        return 0;
    }
    if (words_array[++i][0] == ',') {
        handle_error(EndsWithComma, line_number);
        return 0;
    }

    return 1;  /* Valid */
}
int is_valid_symbol(const char *line, int line_number, int *error_found, char words_array[LEN][LEN]) {
    /* Skip initial white spaces */
    while (isspace(*line)) {
        line++;
    }

    /*  Check if the first character is a letter */
    if (!isalpha(*line)) {
        handle_error(SymbolMustStartWithLetter, line_number);
        *error_found = 1;
        return 0;
    }

    /*  Check if the rest of the characters are letters or digits */
    line++;
    while ( isalnum(*line) ) {
        line++;
    }

    /*  Check for ':' after the symbol name */
    if ( *line != ':' ) {
        *error_found = 1;
        handle_error(SymbolMustEndWithColon, line_number);
        return 0;
    }

    if ( is_reserved_keyword(words_array[0]) ){
        *error_found = 1;
        handle_error(SymbolIsReservedWord, line_number);
        return 0;
    }

    if ( words_array[2] == '\0' ){
        *error_found = 1;
        handle_error(EmptySymbolDefinition, line_number);
        return 0;
    }


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
                encode_second_and_third_word(&instructions_array[IC + 1], NUMBER, operand1,0);
                return 2; /* two lines of instructions were added */
            } else { /* number of operands is 2 */
                write_source_addressing(&instructions_array[IC], 1);
                /* A,R,E of second word is 0 */
                encoding_A_R_E(&instructions_array[IC + 1], 0);
                encode_second_and_third_word(&instructions_array[IC + 1], NUMBER, operand1,number_of_operands);
            }
            break;
        case REGISTER:
            /* A,R,E of second word is 0 */
            encoding_A_R_E(&instructions_array[IC + 1], 0);

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
            insertNode(pass2_list_head,createNode(operand1,IC + 1,line_number) );
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
            insertNode(pass2_list_head,createNode(operand2,IC + 2,line_number) );
            return 3;
    }
    return 0; /* random value change laterrrrrrrrrrrrrrrr */
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
    if (operation_code == NULL_OP ){
        handle_error(UnknownOperationName,line_number);
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
    if (number_of_operands == 1 || number_of_operands == 0){
        if ( words_array[instruction_index + number_of_operands + 1] != NULL ){
            handle_error(ExcessCharactersInInstruction, line_number);
            return 0;
        }
    } else {
        if ( words_array[instruction_index + 4][0] != '\0') {
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

    if(!is_valid){
        handle_error(UndefinedOperandsNames,line_number);
        return 0;
    }

    if ( !valid_addressing_for_operands(operand1_type, operand2_type, operation_code, number_of_operands) ){
        handle_error(InvalidOperandAddressing,line_number);
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



/*  STRUCT HANDLING FUNCTIONS */

/* Function to create a new node */
struct SymbolNameAndIndex* createNode(const char *name, int IC_index, int line_number) {
    struct SymbolNameAndIndex* newNode = malloc(sizeof(struct SymbolNameAndIndex));
    if (newNode == NULL) {
        handle_error(FailedToAllocateMemory, line_number);
        return NULL;
    }

    newNode->name = my_strdup(name);  /* Duplicate the string to avoid issues */
    newNode->IC_index = IC_index;
    newNode->line_number = line_number;  /* Set line number */
    newNode->next = NULL;
    return newNode;
}
/* Function to insert a node at the end of the list */
void insertNode(struct SymbolNameAndIndex** head, struct SymbolNameAndIndex* newNode) {
    if (*head == NULL) {
        *head = newNode;
        return;
    }

    struct SymbolNameAndIndex* current = *head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = newNode;
}
/* Function to free the linked list and its nodes */
void freeList(struct SymbolNameAndIndex* head) {
    struct SymbolNameAndIndex* current = head;
    while (current != NULL) {
        struct SymbolNameAndIndex* temp = current;
        current = current->next;
        free(temp);  /* Deallocate the memory for the current node, including the name string */
    }
}
/* Function to get the name at a specified index */
char* get_name_at(const struct SymbolNameAndIndex* head, int targetIndex) {
    const struct SymbolNameAndIndex* current = head;
    while (current != NULL) {
        if (current->IC_index == targetIndex) {
            return current->name;
        }
        current = current->next;
    }
    return NULL;  /* Index not found */
}
/* Function to get the index at a specified index */
int get_index_at(const struct SymbolNameAndIndex* head, int targetIndex) {
    const struct SymbolNameAndIndex* current = head;
    while (current != NULL) {
        if (current->IC_index == targetIndex) {
            return current->IC_index;
        }
        current = current->next;
    }
    return -1;  /* Index not found */
}
/* Function to get the line number at a specified index */
int get_line_number_at(const struct SymbolNameAndIndex* head, int targetIndex) {
    const struct SymbolNameAndIndex* current = head;
    while (current != NULL) {
        if (current->IC_index == targetIndex) {
            return current->line_number;
        }
        current = current->next;
    }
    return -1;  // Index not found
}




/* DIRECTIVE HANDLING FUNCTIONS */
int is_directive(char *word, const char *directive) {
    if (strcmp(word, directive) == 0) {
        return 1;
    }
    return 0;
}
int is_valid_char(char c) {
    return (c >= 32 && c <= 126);
}
char *custom_strrchr(const char *str, int ch) {
    const char *last = NULL;

    while (*str != '\0') {
        if (*str == ch) {
            last = str;
        }
        str++;
    }

    return (char *)last;
}
int valid_string_directive(char *line, int line_number, int *error_found, int words_to_skip) {
    char *directive = ".string";
    char quote = '\"';
    char *start = line;
    char *last_non_whitespace = NULL;
    char *opening_quote = strchr(start, '"');
    char *closing_quote = NULL;
    int i = 0;

    /* Skip leading whitespace */
    while (*start != '\0' && is_whitespace(*start)) {
        start++;
    }

    for (i; i < words_to_skip + 1; i++) {
        /* skip first word and second word */
        while (*start != '\0' && !is_whitespace(*start)) {
            start++;
        }

        if (*start == '\0') {
            *error_found = 1;
            handle_error(InvalidFormat, line_number);
            return 0;
        }

        /* Skip whitespace between words */
        while (*start != '\0' && is_whitespace(*start)) {
            start++;
        }
    }

    /* Missing opening quote */
    if (!opening_quote) {
        *error_found = 1;
        handle_error(MissingStartOfStringSign, line_number);
        return 0;
    }

    /* Check characters before the directive */
    if (*start != quote){
        *error_found = 1;
        handle_error(NonWhiteCharsBeforeQuote,line_number);
        return 0;
    }

    /* Check characters within the string */
    closing_quote = custom_strrchr(opening_quote + 1, '"');
    if (!closing_quote) {
        *error_found = 1;
        handle_error(MissingEndOfStringSign, line_number);
        return 0;
    }

    for (char *ptr = opening_quote + 1; ptr < closing_quote; ptr++) {
        if (!is_valid_char(*ptr)) {
            *error_found = 1;
            handle_error(InvalidCharInString, line_number);
            return 0;
        }
    }

    /* Check the characters after closing quote */
    char *after_closing_quote = closing_quote + 1;
    while (*after_closing_quote != '\0') {
        if (!is_whitespace(*after_closing_quote)) {
            *error_found = 1;
            handle_error(NonWhiteCharsAfterClosingQuote, line_number);
            return 0;
        }
        after_closing_quote++;
    }
    return 1;
}
void write_character(struct DataStructure* data, char c) {
    /* Convert the char to int and store it in the struct */
    data->value = (int)c;
}
int handle_string_directive(struct DataStructure *data_array, int DC, char *line, int line_number) {
    char *start = line;
    char *closing_quote = custom_strrchr(start, '"'); /* Find the last quotation mark using custom_strrchr */
    int chars_inserted = 0;
    /* Skip to the first opening quote */
    while (*start != '\0' && *start != '"') {
        start++;
    }
    start++; /* Move past the opening quote */

    while (start < closing_quote) {
        write_character(&data_array[DC], *start);
        DC++;
        chars_inserted++;
        start++;
    }

    /* Insert null terminator at the end of the string */
    write_character(&data_array[DC], '\0');
    chars_inserted++;

    return chars_inserted;
}
void write_integer(struct DataStructure* data, int number) {
    data->value = number & 0xFFF; /* Mask the value to keep only the 12 least significant bits (LSBs) */
}
int valid_number_range(char *word) {
    int value = atoi(word);

    /* valid 12 bits integer values */
    if (value < -2048 || value > 2047) {
        return 0;
    }

    return 1; /* No error */
}
int valid_commas_in_directive(char words_array[LEN][LEN], int starting_index, int line_number) {
    int i = starting_index;

    /* Check if the first word is a comma */
    if (words_array[i][0] == ',') {
        handle_error(CommaAtStart,line_number);  /* Comma at start */
        return 0;  /* Invalid */
    }

    i++;  /* Move to the next word */

    /* Iterate through the words */
    while (words_array[i][0] != '\0') {
        /* Check if the current word is a comma */
        if (words_array[i][0] == ',') {
            /* Check for consecutive commas */
            if (words_array[i - 1][0] == ',') {
                handle_error(ConsecutiveCommas,line_number);  /* 2 consecutive commas */
                return 0;  /* Invalid */
            }
        } else {
            /* Check if the previous word is not a comma */
            if (words_array[i - 1][0] != ',') {
                handle_error(MissingComma,line_number);  /* Comma expected after a word */
                return 0;  /* Invalid */
            }
        }

        i++;  /* Move to the next word */
    }

    /* Check if the last word is a comma */
    if (words_array[i - 1][0] == ',') {
        handle_error(EndsWithComma,line_number);  /* Ends with comma error */
        return 0;  /* Invalid */
    }

    return 1;  /* Valid */
}
int valid_data_directive(char words_array[LEN][LEN], int line_number, int *error_found, int symbol_definition) {
    int index = 1; /* Start IC_index for checking words_array */

    /* Adjust index if symbol is defined */
    if (symbol_definition) {
        index = index + 2;
    }

    /* Check if commas are valid in the line */
    if (valid_commas_in_directive(words_array, index, line_number) ) {
        /* Iterate through words_array in steps of 2 */
        for (index; words_array[index][0] != '\0'; index += 2) {
            /* Check if the current word is not a number */
            if (!is_number(words_array[index])) {
                handle_error(InvalidCharInDataDirective, line_number);
                *error_found = 1; /* Set error flag */
                return 0;         /* Directive is not valid */
            }

            /* Check the number range */
            if (!valid_number_range(words_array[index]) ) {
                handle_error(IntegerOverflow, line_number);
                *error_found = 1; /* Set error flag */
                return 0;         /* Directive is not valid */
            }
        }
    } else {
        *error_found = 1; /* Set error flag */
        return 0;         /* Directive is not valid */
    }

    return 1; /* Directive is valid */
}
int handle_data_directive(struct DataStructure *data_array, int DC, char words_array[LEN][LEN], int symbol_definition) {
    int data_index = 1;
    int values_inserted = 0;

    /* if there is a symbol then we should skip 2 words because words_array separates symbol name and the char ':'  */
    if (symbol_definition)
        data_index = data_index + 2;

    while (words_array[data_index][0] != '\0') {
        int value = atoi(words_array[data_index]);

        write_integer(&data_array[DC], value);
        values_inserted++;
        DC++;

        /* Move to the next word if it's a comma */
        if (words_array[data_index + 1][0] == ',') {
            data_index += 2; /* Skip the comma and move to the next word */
        } else {
            data_index++; /* Move to the next word */
        }
    }

    return values_inserted;
}




/* EXTERN AND ENTRY FUNCTIONS */
int valid_entry_and_extern_directive(char words_array[LEN][LEN], int *error_found, int line_number, int symbol_definition){
    int index = 1;

    /* if there is symbol definition skip the name of the symbol and ':' char */
    if(symbol_definition)
        index = index + 2;

    if( !valid_commas_in_directive(words_array,index,line_number) ){
        *error_found = 1;
        return 0;
    }

    if (words_array[index][0] == '\0'){
        *error_found = 1;
        handle_error(InvalidNumberOfOperands,line_number);
        return 0;
    }

    while ( words_array[index][0] != '\0'){
        if( !is_symbol(words_array[index]) ){
            *error_found = 1;
            handle_error(OnlySymbolsAllowed,line_number);
            return 0;
        }
        /* check the next parameter, skipping the: ',' char */
        index = index + 2;
    }
    /* passed all checks */
    return 1;
}
void handle_extern_and_entry_directives(char words_array[LEN][LEN], Symbol **symbol_head, int symbol_definition,int line_number, int *error_found, SymbolType type, SymbolCategory category ){
    int index = 1;

    if(symbol_definition)
        index = index + 2;

    while (words_array[index][0] != '\0'){
        handle_symbol(symbol_head,words_array[index],line_number,error_found, category, type,0);
        index = index + 2;
    }
}



/* FORMAT LINE */
int is_whitespace(char c) {
    return c == ' ' || c == '\t';
}
/* valid line has 80 char + '\n' char */
int line_too_long(char *line){
    if (strlen(line) <= 81)
        return 1;
    return 0;
}
void format_line(char source[LEN], char formatted_line[LEN]) {
    /* Get the length of the input line */
    int i = 0;
    int len = (int)strlen(source);

    /* Variables to keep track of the formatted line */
    int formatted_index = 0;
    int last_was_space = 1;

    /* Iterate through each character of the input line */
    for (i; i < len; i++) {
        char current_char = source[i];
        int is_space = is_whitespace(current_char);

        /* Handle spaces and tabs */
        if (is_space) {
            if (!last_was_space && formatted_index > 0 && formatted_line[formatted_index - 1] != ',' && formatted_line[formatted_index - 1] != ':') {
                formatted_line[formatted_index++] = ' '; /* Replace multiple spaces with a single space */
            }
            last_was_space = 1;
        }
            /* Handle commas and colons */
        else if (current_char == ',' || current_char == ':') {
            if (!last_was_space) {
                if (formatted_index > 0 && formatted_line[formatted_index - 1] != ',' && formatted_line[formatted_index - 1] != ':') {
                    formatted_line[formatted_index++] = ' '; /* Add a space before comma/colon if there wasn't one */
                }
            }

            formatted_line[formatted_index++] = current_char;
            formatted_line[formatted_index++] = ' '; /* Add a space after comma/colon */
            last_was_space = 1;
        }
            /* Handle regular characters */
        else {
            formatted_line[formatted_index++] = current_char;
            last_was_space = 0;
        }
    }

    /* Remove trailing whitespace, if any */
    if (formatted_index > 0 && is_whitespace(formatted_line[formatted_index - 1])) {
        formatted_index--;
    }

    /* Null-terminate the formatted string */
    formatted_line[formatted_index] = '\0';
}
void store_words(char line[], char resultArray[LEN][LEN]) {
    char* token;
    const char delimiter[] = " ";
    int index = 0;

    memset(resultArray, 0, sizeof(resultArray[0][0]) * LEN * LEN);

    token = strtok(line, delimiter);

    while (token != NULL && index < LEN - 1) {
        if (strlen(token) < LEN) {
            strncpy(resultArray[index], token, LEN - 1);
            resultArray[index][LEN - 1] = '\0';
            index++;
        }
        token = strtok(NULL, delimiter);
    }
}
void format_and_store_words(char line[LEN],char words_array[LEN][LEN]){
    char formatted_line[LEN] =  {0};
    format_line(line,formatted_line);
    store_words(formatted_line,words_array);
}
void update_variables(char **current_symbol_name, int *symbol_definition, int *line_number, int *index){
    *(current_symbol_name) = NULL;
    *symbol_definition = 0;
    *line_number = *line_number + 1;
    *index = 0;
}
int is_comment(const char* line) {
    int index = 0;

    while (isspace(line[index])) {
        index++; /* Skip leading whitespace characters. */
    }

    return (line[index] == ';') ? 1 : 0;
}
/* Function to check if a given line is empty (contains only whitespace characters) */
int is_empty(const char *line) {
    while (*line != '\0') {
        /* Loop through each character in the line until the end of the string is reached. */

        if (!isspace((unsigned char)*line)) {
            /*
             * Check if the current character is not a whitespace character.
             * The `(unsigned char)` cast is used to handle potential negative char values correctly.
             */

            return 0; /* Return 0 immediately if a non-whitespace character is found. */
        }

        line++; /* Move to the next character in the line. */
    }

    return 1; /* Return 1 if the loop reaches the end of the line without finding a non-whitespace character. */
}
/* Function to skip a line if it is a comment or empty */
int comment_or_empty(char *line)
{
    return ( is_comment(line) || is_empty(line));
}




/* SYMBOL STRUCT FUNTIONS */
void handle_symbol(Symbol **head, const char *name, int line_number, int *error_found, SymbolType parameter_type, SymbolCategory parameter_entry_or_extern, int parameter_value) {
    Symbol *existing_symbol = NULL;
    Symbol *new_symbol = NULL;
    if (head == NULL || name == NULL) {
        return; /* Safeguard against invalid inputs */
    }

    /* Check if symbol with the given name already exists */
    existing_symbol = find_symbol(*head, name);
    if (existing_symbol != NULL) {
        /* mark existing symbol as extern or entry */
        switch (existing_symbol->category ){
            case(NONE_CATEGORY):
                if (parameter_entry_or_extern == ENTRY)
                    existing_symbol->type = ENTRY;
                else if (parameter_entry_or_extern == EXTERN){
                    handle_error(RedefinitionOfSymbolType,line_number);
                    *error_found = 1;
                    return;
                } else if (parameter_entry_or_extern == NONE_CATEGORY){
                    handle_error(MultipleSymbolDefinition,line_number);
                    *error_found = 1;
                    return;
                }
                break;
            case(ENTRY):
                if (parameter_entry_or_extern == EXTERN){
                    handle_error(RedefinitionOfSymbolType,line_number);
                    *error_found = 1;
                    return;
                }
                else if (existing_symbol->val == 0 && parameter_entry_or_extern ==  NONE_CATEGORY){
                    existing_symbol->val = parameter_value;
                }
                else if (parameter_entry_or_extern == ENTRY) {} /* do nothing */
                else {
                    handle_error(MultipleSymbolDefinition,line_number);
                    *error_found = 1;
                    return;
                }
                break;
            case(EXTERN):
                if (parameter_entry_or_extern == ENTRY){
                    handle_error(RedefinitionOfSymbolType,line_number);
                    *error_found = 1;
                    return;
                }else if (parameter_entry_or_extern == EXTERN){} /* do nothing */
                else{
                    handle_error(RedefinitionOfExternSymbol,line_number);
                    *error_found = 1;
                    return;
                }
                break;

        }

    }
        /* Create a new symbol and handle memory allocation errors */
        new_symbol = create_symbol(name, parameter_value, parameter_entry_or_extern, parameter_type);
        if (new_symbol == NULL) {
            handle_error(FailedToAllocateMemory, line_number);
            *error_found = 1;
            return;
        }
        new_symbol->next = *head; /* Link the new symbol to the current head */
        *head = new_symbol; /* Set head to the new symbol */
}
Symbol *find_symbol(Symbol *head, const char *name) {
    Symbol *current = head;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL; // Symbol not found
}
int is_in_symbol_list(Symbol *head, const char *name) {
    Symbol *symbol = find_symbol(head, name);
    return (symbol != NULL);
}
Symbol *create_symbol(const char *name, int val, SymbolType type, SymbolCategory category) {
    Symbol *new_symbol = (Symbol *)malloc(sizeof(Symbol));
    if (new_symbol != NULL) {
        new_symbol->name = my_strdup(name); /* Allocate and copy name */
        new_symbol->val = val;
        new_symbol->type = type;
        new_symbol->category = category;
        new_symbol->next = NULL;
    }
    return new_symbol;
}

int is_in_symbol_list(Symbol *head, const char *name) {
    Symbol *symbol = find_symbol(head, name);
    return (symbol != NULL);
}

void free_symbol_list(Symbol *head) {
    while (head != NULL) {
        Symbol *temp = head;
        head = head->next;
        free(temp->name);
        free(temp);
    }
}

char *my_strdup(const char *str) {
    size_t length = strlen(str) + 1; /* Include space for the null terminator */
    char *duplicate = (char *)malloc(length);

    if (duplicate != NULL) {
        strcpy(duplicate, str);
    }

    return duplicate;
}