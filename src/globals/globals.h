#ifndef __GLOBALS_H
#define __GLOBALS_H

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
    char encoding_type : 2;    /* 2-bit field */
    char target_addressing : 3; /* 3-bit field */
    char opcode : 4;         /* 4-bit field */
    char source_addressing : 3; /* 3-bit field */
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


#endif