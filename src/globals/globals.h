#ifndef __GLOBALS_H
#define __GLOBALS_H

#define LEN 200
#define INITIAL_VAL 10
#define RESERVED_KEYWORDS_LENGTH 16
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../error-handling/errors.h"

extern const char *reservedKeywords[RESERVED_KEYWORDS_LENGTH];

typedef enum {
    CODE,
    DATA,
    NONE_TYPE
} SymbolType;

typedef enum {
    EXTERN,
    ENTRY,
    NONE_CATEGORY
} SymbolCategory;

typedef struct Symbol {
    char *name;
    int val;
    SymbolType type;
    SymbolCategory category;
    struct Symbol *next;
} Symbol;

/* in c90 standard minimum width of int is 16 bits, so we can use the first 12 bits of it*/
struct DataStructure {
    int value : 12;
};

struct SymbolNameAndIndex {
    char *name;
    int IC_index;
    int line_number;
    struct SymbolNameAndIndex *next;
};

/* in c90 standard minimum width of char is 8 bits */
struct InstructionStructure {
    char encoding_type : 2;     /* 2-bit field */
    char target_addressing : 3; /* 3-bit field */
    char opcode : 4;            /* 4-bit field */
    char source_addressing : 3; /* 3-bit field */
};

enum OperandType {
    NUMBER = 1,
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

/* functions */
void sanitize_input(char *input);
char *my_strdup(const char *str);
FILE *create_file(const char *file_name, const char *extension);
int is_reserved_keyword(const char *word);

/* structure functions*/
struct SymbolNameAndIndex *get_node_by_name(struct SymbolNameAndIndex *head, const char *target_name);
struct SymbolNameAndIndex *create_node(const char *name, int IC_index, int line_number);
void insertNode(struct SymbolNameAndIndex **head, struct SymbolNameAndIndex *newNode);
void free_list(struct SymbolNameAndIndex *head);
char *get_name_at(const struct SymbolNameAndIndex *head, int targetIndex);
int get_index_at(const struct SymbolNameAndIndex *head, int targetIndex);
int get_line_number_at(const struct SymbolNameAndIndex *head, int targetIndex);

/* symbol list */
void free_symbol_list(Symbol *head);
int is_valid_symbol(const char *line, int line_number, int *error_found, char words_array[LEN][LEN]);
int is_in_symbol_list(Symbol *head, const char *name);

Symbol *copy_symlist(Symbol *original);
Symbol *find_symbol(Symbol *head, const char *name);
Symbol *create_symbol(const char *name, int val, SymbolType type, SymbolCategory category);

#endif