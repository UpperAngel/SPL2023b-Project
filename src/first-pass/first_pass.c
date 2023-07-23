#include "first-pass-headers/first_pass.h"

enum opcodes {
    /* First Group */
    MOV_OP = 0,
    CMP_OP = 1,

    ADD_OP = 2,
    SUB_OP = 2,

    LEA_OP = 4,
    /* END First Group */

    /* Second Group */
    CLR_OP = 5,
    NOT_OP = 5,
    INC_OP = 5,
    DEC_OP = 5,

    JMP_OP = 9,
    BNE_OP = 9,
    JSR_OP = 9,

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

struct command {
    const char *name;
    opcodes opcode;
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
    {NULL, NULL_OP},
};

struct encoded_instruction {
    unsigned int encoding_type : 2;
    unsigned int target_addressing : 3;
    unsigned int opcode : 4;
    unsigned int source_addressing : 3;
};

opcodes get_opcode(char *command) {
    int i;

    if (command == NULL) {
        return NULL_OP;
    }

    for (i = 0; commands[i].name != NULL; i++) {
        if (strcmp(command, commands[i].name) == 0) {
            return commands[i].opcode;
        }
    }

    return NULL_OP;
}

char *get_label(const char *line) {
    char *label = NULL;

    char *mod_line = strdup(line);  // Use strdup to duplicate the line

    label = strtok(mod_line, ":");
    if (label != NULL) {
        // Don't concatenate ":" to label, just return label
        return label;
    }
    // Clean up and return NULL if no label found
    free(mod_line);
    return NULL;
}

int is_valid_label(const char label[31]) {
    char c;
    int i = 0;
    int j;

    while ((c = label[i]) != '\0') {
        if (i == 0 && isdigit(c)) {
            return 0;
        }
        if (!isalnum(label[i])) {
            return 0;
        }
        i++;
    }

    /* checks command names */
    for (j = 0; commands[j].name != NULL; j++) {
        if (strcmp(label, commands[j].name) == 0) {
            return 0;
        }
    }

    return 1;
}


int get_encoding(opcodes opcode) {

}

encoded_instruction *encode_instruction(char *instruction_line) {
    encoded_instruction *ret;
    char *token;
    int token_num = 0;

    ret = malloc(sizeof(encoded_instruction));
    token = strtok(instruction_line, " ");
    token_num += 1;



    while ((token = strtok(NULL, " ")) != NULL) {
        switch (token_num)
        {
        case 1:
            ret->opcode = get_opcode(token);
            break;
        case 2:
            ret->source_addressing = 0;
            break;
        case 3:
            ret->target_addressing = 0;
            break;
        default:
            break;
        }

        token_num++;
    }
    return ret;
}