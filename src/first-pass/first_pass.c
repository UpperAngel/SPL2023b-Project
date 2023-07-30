#include "first-pass-headers/first_pass.h"

#include "first-pass-headers/instruction.h"
#include "first-pass-headers/symbol_table.h"

struct command
{
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

/* function to get a opcode from a command name */

Opcode get_opcode(char *command)
{
    int i;

    if (command == NULL)
    {
        return NULL_OP;
    }

    for (i = 0; commands[i].name != NULL; i++)
    {
        if (strcmp(command, commands[i].name) == 0)
        {
            return commands[i].opcode;
        }
    }

    return NULL_OP;
}
/* function to get the label out of a line */

int is_label(const char *line)
{
    char *dots = NULL;

    dots = strchr(line, ':');
    if (dots != NULL)
    {
        return 1;
    }
    
    return 0;
}

/* checks if a label is valid */
int is_valid_label(const char label[31])
{
    char c;
    int i = 0;
    int j;

    while ((c = label[i]) != '\0')
    {
        if (i == 0 && isdigit(c))
        {
            return 0;
        }
        if (!isalnum(label[i]))
        {
            return 0;
        }
        i++;
    }

    /* checks command names */
    for (j = 0; commands[j].name != NULL; j++)
    {
        if (strcmp(label, commands[j].name) == 0)
        {
            return 0;
        }
    }

    return 1;
}
