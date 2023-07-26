#include "first-pass-headers/instruction.h"

#include "first-pass-headers/symbol_table.h"

#define DIRECTIVES_NUM 4
struct encodedInstruction {
    unsigned int encoding_type : 2;
    unsigned int target_addressing : 3;
    unsigned int opcode : 4;
    unsigned int source_addressing : 3;
};


static char *directives[] = {
    "extern",
    "string",
    "data",
    "entry",
};

char *get_content(const char *line) {
    char *content = strchr(line, ':');

    if (content == NULL) {
        return NULL;
    }
    content++;

    return content;
}

/* function to encode a instruction into a 12bit word. */
EncodedInstruction *encode_instruction(const char *instruction_line, const SymbolTable *table) {
    char *mod_line;
    EncodedInstruction *ret;

    char *token;
    int token_num = 0;

    mod_line = strdup(instruction_line);
    ret = malloc(sizeof(EncodedInstruction));

    token = strtok(mod_line, " ");
    token_num += 1;

    while ((token = strtok(NULL, " ")) != NULL) {
        switch (token_num) {
            case 1:
                ret->opcode = get_opcode(token);
                break;
            case 2:
                ret->source_addressing = get_addressing(token, table);
                break;
            case 3:
                ret->target_addressing = get_addressing(token, table);
                break;
            default:
                break;
        }

        token_num++;
    }
    return ret;
}

int is_directive(const char *line) {
    int i;
    
    char *directive;
    char *dot = strchr(line, '.');
    if (dot == NULL)
    {
        return 0;
    }
    
    directive = dot++;
    for (i = 0; i < DIRECTIVES_NUM; i++)
    {
        if (strcmp(directive, directives[i]) == 0)
        {
            return 1;
        }
    }
    return 0;
    
    
    
}