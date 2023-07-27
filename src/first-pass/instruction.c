#include "first-pass-headers/instruction.h"

#include "first-pass-headers/symbol_table.h"

#define DIRECTIVES_NUM 4

enum addressingType {
    ERROR = 0,
    DIRECT = 3,
    IMMEDIATE = 5,
    REGISTER = 1,
};

struct encodedInstruction {
    unsigned int encoding_type : 2;
    unsigned int target_addressing : 3;
    unsigned int opcode : 4;
    unsigned int source_addressing : 3;
};

static char *directives[DIRECTIVES_NUM] = {
    "extern",
    "string",
    "data",
    "entry",
};

int is_number(const char *str) {
    int num = atoi(str);

    return num != 0 || *str == '0';
}

enum addressingType get_addressing(const char *operand, SymbolTable *table) {
    int i;
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

    if (is_number(operand))
    {
        return IMMEDIATE;
    }
    

    if (get_symbol_by_name(table, operand) != NULL) {
        return DIRECT;
    }
    return ERROR;
}
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

int calc_l(const char *line, SymbolTable *table) {
    int i = 0, line_word_num = 0, bin_word_num = 0;
    char *mod_line = strdup(line);
    char *words[3];

    enum addressingType first_op_addressing;
    enum addressingType second_op_addressing;

    char *word = strtok(mod_line, " ");
    line_word_num++;

    while (word != NULL) {
        if (strchr(word, ",")) {
            continue;
        }

        words[i] = word;
        
        line_word_num++;
        i++;
        
        word = strtok(NULL, " ");
    }

    first_op_addressing = get_addressing(words[1], table);
    second_op_addressing = get_addressing(words[2], table);
    if (first_op_addressing == REGISTER && second_op_addressing == REGISTER)
    {
        return 1;
    }
    else 
    {
        return 2;
    }
}

/* NEED TO SEPRATE TO DATA AND OTHER TYPES */
int is_directive(const char *line) {
    int i;

    char *directive;
    char *dot = strchr(line, '.');
    if (dot == NULL) {
        return 0;
    }

    directive = dot++;
    for (i = 0; i < DIRECTIVES_NUM; i++) {
        if (strcmp(directive, directives[i]) == 0) {
            return 1;
        }
    }
    return 0;
}
