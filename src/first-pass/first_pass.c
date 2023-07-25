#include "first-pass-headers/first_pass.h"

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
enum addressingType {
    ERROR = 0,
    DIRECT = 3,
    IMMEDIATE = 5,
    REGISTER = 1,
};
struct command {
    const char *name;
    Opcode opcode;
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

struct encodedInstruction {
    unsigned int encoding_type : 2;
    unsigned int target_addressing : 3;
    unsigned int opcode : 4;
    unsigned int source_addressing : 3;
};

enum addressingType get_addressing(const char *operand) {
    int i;
    int digit_count = 0;
    if (operand == NULL) {
        return ERROR;
    }
    /* ADD CHECK FOR LABELS */

    /* @r1 */
    if (operand[0] == '@' && operand[1] == 'r' && isdigit(operand[2])) {
        if (atoi(operand[2]) < 7) {
            return REGISTER;
        }
        return ERROR;
    }

    for (i = 0; i < strlen(operand); i++) {
        if (isdigit(operand[i])) {
            digit_count++;
        }
    }

    if (digit_count == strlen(operand)) {
        atof(operand);
    }
}

Opcode get_opcode(char *command) {
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

int get_encoding(Opcode opcode) {
}

EncodedInstruction *encode_instruction(char *instruction_line) {
    EncodedInstruction *ret;
    char *token;
    int token_num = 0;

    ret = malloc(sizeof(EncodedInstruction));
    token = strtok(instruction_line, " ");
    token_num += 1;

    while ((token = strtok(NULL, " ")) != NULL) {
        switch (token_num) {
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

/* Function to check each line in the source file for length and newline character */
int valid_file_lines(FILE *source_file) {
    char line[82]; /* Allow space for 80 characters + newline + null-terminator */
    int line_number = 1;

    while (fgets(line, sizeof(line), source_file) != NULL) {
        int line_length = strlen(line);

        /* Check if the line length exceeds 80 characters */
        if (line_length > 80) {
           /*  handle_error(LineLengthExceeded, line_number); */
            return 0;
        }

        /* Check if the last character is not '\n' */
        if (line[line_length - 1] != '\n') {
            /* handle_error(MissingNewlineCharacter, line_number); */
            return 0;
        }

        line_number++;
    }
    fseek(source_file, 0L, SEEK_SET);
    return 1;
}

/* Function to check each line in the source file for length and newline character */
int valid_file_lines(FILE *source_file) {
    char line[82]; /* Allow space for 80 characters + newline + null-terminator */
    int line_number = 1;

    while (fgets(line, sizeof(line), source_file) != NULL) {
        int line_length = strlen(line);

        /* Check if the line length exceeds 80 characters */
        if (line_length > 80) {
          /*   handle_error(LineLengthExceeded, line_number); */
            return 0;
        }

        /* Check if the last character is not '\n' */
        if (line[line_length - 1] != '\n') {
            /* handle_error(MissingNewlineCharacter, line_number); */
            return 0;
        }

        line_number++;
    }
    fseek(source_file, 0L, SEEK_SET);
    return 1;
}