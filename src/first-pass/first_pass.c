#include "first-pass-headers/first_pass.h"

#include "first-pass-headers/instruction.h"
#include "first-pass-headers/symbol_table.h"

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

int first_pass(FILE *am_file, FILE *tar_file) {
    int i;

    char *line;
    EncodedInstruction *encoded_instruction;
    SymbolTable *symbol_table;
    char *curr_label;
    char *curr_content;
    SymbolType curr_type = NONE;
    symbol_table = init_table(10);

    while (fgets(line, 100, am_file)) {
        if (line > 82) {
            fprintf(stdout, "line is over the maximum length");
        }

        if (get_label(line) != NULL) {
            curr_label = get_label(line);
            
            if (is_valid_label(curr_label)) {
                curr_content = get_content(line);
                
                if (is_directive(curr_content)) {
                    curr_type == DATA;
                } else {
                    curr_type == CODE;
                }
                
                add_symbol(symbol_table, curr_label, curr_content, curr_type);
            }
            continue;

            /* invalid label name error */
        }

        encoded_instruction = encode_instruction(line, symbol_table);

        fprintf(tar_file, "%s", line);
    }
}

/* function to get a opcode from a command name */

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
/* function to get the label out of a line */

char *get_label(const char *line) {
    char *label = NULL;

    char *mod_line = strdup(line); /* strdup to duplicate the line */

    label = strtok(mod_line, ":");
    if (label != NULL) { /* found label. returns it*/
        return label;
    }

    /* Clean up and return NULL if no label found */
    free(mod_line);
    return NULL;
}

/* checks if a label is valid */
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