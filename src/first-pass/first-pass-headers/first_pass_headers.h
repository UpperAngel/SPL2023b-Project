#ifndef first_pass_headers_H
#define first_pass_headers_H

#define LEN 200
#define INITIAL_VAL 100

#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>



typedef enum {
    CODE,
    DATA,
    NONE_TYPE
} SymbolType;

typedef enum {
    EXTERN,
    ENTRY,
    NONE_CATEGORY
}SymbolCategory;

typedef struct Symbol {
    char *name;
    int val;
    SymbolType type;
    SymbolCategory category;
    struct Symbol *next;
} Symbol;



const char *reservedKeywords[] = {
        "mov", "cmp", "add", "sub", "not", "clr", "lea", "inc",
        "de", "jmp", "bne", "red", "prn", "jsr", "rts", "stop"
};

/* in c90 standard minimum width of int is 16 bits, so we can use the first 12 bits of it*/
struct DataStructure {
    int value : 12;
};

struct SymbolNameAndIndex {
    char *name;
    int IC_index;
    int line_number;
    struct SymbolNameAndIndex* next;
};

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
    UNDEFINED
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


/* ENCODE FUNCTIONS */
void write_opcode(struct InstructionStructure* encoded_inst, enum opcode op) ;
void write_source_addressing(struct InstructionStructure* encoded_inst, unsigned int source_addressing) ;
void write_target_addressing(struct InstructionStructure* encoded_inst, unsigned int target_addressing) ;
void encoding_A_R_E(struct InstructionStructure* encoded_inst, unsigned int encoding_type) ;
void encode_bits_2_to_11(int number, struct InstructionStructure* encoded_inst) ;
void encode_bits_2_to_6(int number, struct InstructionStructure* encoded_inst) ;
void encode_bits_7_to_11(int number, struct InstructionStructure* encoded_inst) ;
void encode_opcode(struct InstructionStructure* encoded_inst, enum opcode operation) ;
void encode_second_and_third_word(struct InstructionStructure *instruction, enum OperandType OPERAND_TYPE, char *operand, int is_source_operand);

/* IS A functions */
int is_register(const char *operand) ;
int is_number(const char *str) ;
int is_symbol(const char *symbol);


/* get functions */
int get_register_number(char *register_str) ;
int get_number_of_operands(Opcode cmd_opcode) ;
enum OperandType get_operand_type( char* operand) ;
enum opcode get_opcode(const char* instruction_name_str);

/* FORAMAT LINE FUNCTIONS */
int line_too_long(char *line);
int is_whitespace(char c) ;
void format_line(char source[LEN], char formatted_line[LEN]) ;
void store_words(char line[], char resultArray[LEN][LEN]) ;
void format_and_store_words(char line[LEN],char words_array[LEN][LEN]);
void update_variables(char **current_symbol_name, int *symbol_definition, int *line_number, int *index);
int comment_or_empty(char *line);
int is_empty(const char *line);
int is_comment(const char* line);




/* STRING DIRECTIVE */
int valid_string_directive(char *line, int line_number, int *error_found, int words_to_skip);
int handle_string_directive(struct DataStructure *data_array, int DC, char *line, int line_number);
int valid_number_range(char *word) ;
int is_valid_char(char c);
int is_directive(char *word, const char *directive);


/* DATA DIRECTIVE */
int valid_data_directive(char words_array[LEN][LEN], int line_number, int *error_found, int symbol_definition);
void write_character(struct DataStructure* data, char c) ;
int handle_data_directive(struct DataStructure *data_array, int DC, char words_array[LEN][LEN], int starting_index);
int is_data_directive(char *word);
void write_integer(struct DataStructure* data, int number);
int valid_commas_in_directive(char words_array[LEN][LEN], int starting_index, int line_number);
int valid_entry_and_extern_directive(char words_array[LEN][LEN], int *error_found, int line_number, int symbol_definition);
void handle_extern_and_entry_directives(char words_array[LEN][LEN], Symbol **symbol_head, int symbol_definition,int line_number, int *error_found, SymbolType type, SymbolCategory category );


/* SYMBOL DEF */
int is_valid_symbol(const char *line, int line_number, int *error_found, char words_array[LEN][LEN]);
int is_reserved_keyword(const char *word);
void handle_symbol(Symbol **head, const char *name, int line_number, int *error_found, SymbolType parameter_type, SymbolCategory parameter_entry_or_extern, int parameter_value);


/* INSTRUCTIONS */
int handle_valid_instruction(char words_array[LEN][LEN], struct InstructionStructure *instructions_array, int IC, int symbol_definition, int line_number, struct SymbolNameAndIndex **pass2_list_head);
int valid_instruction(char words_array[LEN][LEN], int line_number, int symbol_definition);
int valid_addressing_for_operands(enum OperandType operand1_type, enum OperandType operand2_type, Opcode operation_code, int number_of_operands);
int valid_commas_in_instruction(char words_array[LEN][LEN], int starting_index, int line_number);



/* INSTRUCTION HANDLING FUNCTIONS */
struct SymbolNameAndIndex* createNode(const char *name, int IC_index, int line_number);
void insertNode(struct SymbolNameAndIndex** head, struct SymbolNameAndIndex* newNode);
void freeList(struct SymbolNameAndIndex* head);
char* get_name_at(const struct SymbolNameAndIndex* head, int targetIndex);
int get_index_at(const struct SymbolNameAndIndex* head, int targetIndex);
int get_line_number_at(const struct SymbolNameAndIndex* head, int targetIndex);


/* STRUCT OF SYMBOL HANDLING */
Symbol *create_symbol(const char *name, int val, SymbolType type, SymbolCategory category);

#endif
