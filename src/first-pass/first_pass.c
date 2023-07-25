#include "first-pass-headers/first_pass.h"

enum opcode {
    /* group 1 */
    mov_op = 0,
    cmp_op = 1,
    add_op = 2,
    sub_op = 3,
    lea_op = 6,
    /* end of group 1 */

    /* group 2 */
    not_op = 4,
    clr_op = 5,
    inc_op = 7,
    dec_op = 8,
    jmp_op = 9,
    bne_op = 10,
    red_op = 11,
    prn_op = 12,
    jsr_op = 13,
    /* end of group 2 */

    /* group 3*/
    rts_op = 14,
    stop_op = 15,
    /* end of group 3 */

    /* represent a null opcode*/
    null_op = 16,
};

struct cm_entry {
    const char *command;
    Opcode opcode;
};

struct instruction {
    const char *command;
    const char *source_operand;
    const char *target_operand;
};

CommandMap command_map[] = {
    /* group 1 */
    {"mov", mov_op},
    {"cmp", cmp_op},
    {"add", add_op},
    {"sub", sub_op},
    {"lea", lea_op},

    /* group 2 */
    {"not", not_op},
    {"clr", clr_op},
    {"inc", inc_op},
    {"dec", dec_op},
    {"jmp", jmp_op},
    {"bne", bne_op},
    {"red", red_op},
    {"prn", prn_op},
    {"jsr", jsr_op},

    /* group 3*/
    {"rts", rts_op},
    {"stop", stop_op},
    {NULL, null_op},

};

Opcode get_command_opcode(char *command) {
    int i;

    if (command == NULL) /* if the command is null we return the null op (16) */
    {
        return null_op;
    }
    for (i = 0; i < command_map[i].command != NULL; i++) /* if the command is the same as a saved command we return it's
                 opcode.*/
    {
        if (strcmp(command, command_map[i].command) == 0) {
            return command_map[i].opcode;
        }
    }
    return null_op; /* if we don't find the command we return null op */
}

char *get_label(const char *line) {
    char *label = NULL;

    char *mod_line = strdup(line);

    label = strtok(mod_line, ":");
    if (label != NULL) {
        return label;
    }

    // Clean up and return NULL if no label found
    free(mod_line);
    return NULL;
}

int is_valid_label(const char label[32]) {
    int label_len = 32;
    int i, j;

    if (label == NULL) {
        return 0;
    }

    if (!isalpha(label[0])) {
        return 0;
    }

    for (i = 0; i < label_len; i++) {
        if (!isalnum(label[i])) {
            return 0;
        }
    }

    /* checks command names */
    for (j = 0; j < command_map[j].command != NULL; j++) {
        if (strcmp(label, command_map[i].command) != 0) {
            return 0;
        }

        /* ADD CHECK AGAINST LABEL DATABASE */
    }
    return 1;
}