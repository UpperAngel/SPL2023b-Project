#ifndef first_pass_headers_H
#define first_pass_headers_H

#define LEN 200
#define ARRAY_SIZE 1024
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

/* in c90 standard minimum width of char is 8 bits */
struct InstructionStructure {
    char encoding_type : 2;   // 2-bit field
    char target_addressing : 3; // 3-bit field
    char opcode : 4;         // 4-bit field
    char source_addressing : 3; // 3-bit field
};


enum OperandType{
    NUMBER = 0,
    SYMBOL = 3,
    REGISTER = 5,
    NONE
};


/* function to get an opcode from a command name */
enum opcode {
    /* First Group */
    MOV_OP = 0,
    CMP_OP = 1,

    ADD_OP = 2,
    SUB_OP = 3,

    LEA_OP = 6,
    /* END First Group */

    /* Second Group */
    CLR_OP = 5,
    NOT_OP = 4,
    INC_OP = 7,
    DEC_OP = 8,

    JMP_OP = 9,
    BNE_OP = 10,
    JSR_OP = 13,

    RED_OP = 11,
    PRN_OP = 12,
    /* END Second Group */

    /* Third Group */
    RTS_OP = 14,
    STOP_OP = 15,
    /* END Third Group */

    /* Failed/Error */
    NULL_OP = -1
};

typedef enum opcode Opcode;

struct command {
    const char* instruction_name;
    enum opcode opcode_value;
};

static struct command commands[] = {
        {"mov", MOV_OP},
        {"cmp", CMP_OP},
        {"add", ADD_OP},
        {"sub", SUB_OP},
        {"lea", LEA_OP},
        {"clr", CLR_OP},
        {"not", NOT_OP},
        {"inc", INC_OP},
        {"dec", DEC_OP},
        {"jmp", JMP_OP},
        {"bne", BNE_OP},
        {"jsr", JSR_OP},
        {"red", RED_OP},
        {"prn", PRN_OP},
        {"rts", RTS_OP},
        {"stop", STOP_OP},
        {NULL, NULL_OP}
};


/* FUNCTION DECLARATION */


void assign_operands(char words_array[LEN][LEN], char **operand1, char **operand2, int symbol_definition, int number_of_operands);
enum opcode translate_instruction_name(const char* instruction_name_str);

/* ENCODE FUNCTIONS */
void write_opcode(struct InstructionStructure* encoded_inst, enum opcode op) ;
void write_source_addressing(struct InstructionStructure* encoded_inst, unsigned int source_addressing) ;
void write_target_addressing(struct InstructionStructure* encoded_inst, unsigned int target_addressing) ;
void write_encoding_type(struct InstructionStructure* encoded_inst, unsigned int encoding_type) ;
void encode_bits_2_to_11(int number, struct InstructionStructure* encoded_inst) ;
void encode_bits_2_to_6(int number, struct InstructionStructure* encoded_inst) ;
void encode_bits_7_to_11(int number, struct InstructionStructure* encoded_inst) ;
void encode_opcode(struct InstructionStructure* encoded_inst, enum opcode operation) ;
void encode_second_and_third_word(struct InstructionStructure *instruction, enum OperandType OPERAND_TYPE, char *operand, int is_source_operand);

/* IS A functions */
int is_symbol(const char *word);
int is_register(const char *operand) ;
int is_number(const char *str) ;


/* get functions */
int get_register_number(char *register_str) ;
int get_number_of_operands(Opcode cmd_opcode) ;
Opcode get_opcode(char *command) ;
enum OperandType get_operand_type( char* operand) ;

/* FORAMAT LINE FUNCTIONS */
int is_whitespace(char c) ;
void format_line(char source[LEN], char formatted_line[LEN]) ;
void store_words(char line[], char resultArray[LEN][LEN]) ;
void format_and_store_words(char line[LEN],char words_array[LEN][LEN]);
void update_variables(char **current_symbol_name, int *symbol_definition, int *line_number, int *index);
int comment_or_empty(char *line);
int is_empty(const char *line);
int is_comment(const char* line);
int handle_valid_instruction(char words_array[LEN][LEN], struct InstructionStructure *instructions_array, int IC, int symbol_definition);

#endif
