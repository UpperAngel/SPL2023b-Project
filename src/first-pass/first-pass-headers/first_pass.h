#ifndef __FIRST_PASS_H
#define __FIRST_PASS_H

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
enum opcode {
    /* First Group */
    MOV_OP = 0,
    CMP_OP = 1,

    ADD_OP = 2,
    SUB_OP = 2,

    LEA_OP = 4,
    /* END First Group */

    /* Second Group */
    CLR_OP = 5,
    NOT_OP = 6,
    INC_OP = 7,
    DEC_OP = 8,

    JMP_OP = 9,
    BNE_OP = 10,
    JSR_OP = 11,

    RED_OP = 12,
    PRN_OP = 13,
    /* END Second Group */

    /* Third Group */
    RTS_OP = 14,
    STOP_OP = 15,
    /* END Third Group */

    /* Failed/Error */
    NULL_OP = -1
};

typedef enum opcode Opcode;
typedef struct cmEntry CommandMap;
char *get_label(const char *);
int is_valid_label(const char[31]);
Opcode get_opcode( char *);

#endif