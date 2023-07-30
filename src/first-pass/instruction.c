#include "first-pass-headers/instruction.h"
#include "first-pass-headers/symbol_table.h"
#include "first-pass-headers/first_pass.h"

#define DIRECTIVES_NUM 4
#define LINE_LEN 82

static char *directives[DIRECTIVES_NUM] = {
    ".string",
    ".data",
    ".extern",
    ".entry",
};

int is_number(const char *str)
{
    if (*str == '\0')
    {
        return 0; // Empty string is not a number
    }
    // Check for non-numeric characters
    while (*str)
    {
        if (!isdigit(*str))
        {
            return 0; // Non-numeric character found
        }
        str++;
    }

    return 1; // All characters were digits, so it's a number
}

enum addressingType get_addressing(const char *operand, const SymbolTable *table)
{
    int i;
    int op;
    if (operand == NULL)
    {
        return ERROR;
    }

    /* @r1 */
    else if (operand[0] == '@' && operand[1] == 'r' && isdigit(operand[2]))
    {
        if ((operand[2] - 'a') < 7)
        {
            return REGISTER;
        }
        return ERROR;
    }

    else if (is_number(operand))
    {
        return IMMEDIATE;
    }
    else if (is_valid_label(operand))
    {
        return DIRECT;
    }

    return ERROR;
}
char *get_content(const char *line)
{
    char *content = strchr(line, ':');

    if (content == NULL)
    {
        return NULL;
    }
    content++;

    return content;
}

/* function to encode a instruction into a 12bit word. */
int encode_instruction(EncodedInstruction *encoded, const char *instruction_line, const SymbolTable *table)
{

    char opcode_str[5];
    char source_operand[10];
    char target_operand[10];

    // Parse the instruction line
    sscanf(instruction_line, "%s %s, %s", opcode_str, source_operand, target_operand);

    // Get opcode and addressing modes
    Opcode opcode = get_opcode(opcode_str);
    enum addressingType source_addressing = get_addressing(source_operand, table);
    enum addressingType target_addressing = get_addressing(target_operand, table);

    // Check for invalid opcode or addressing modes
    if (opcode == NULL_OP || source_addressing == ERROR || target_addressing == ERROR)
    {
        return 0;
    }

    // Create the encoded instruction
    encoded = (EncodedInstruction *)malloc(sizeof(EncodedInstruction));
    if (source_addressing == REGISTER && target_addressing == REGISTER)
    {
        encoded->encoding_type = 1;
    }
    else if (source_addressing == DIRECT || target_addressing == DIRECT)
    {
        encoded->encoding_type = 2;
    }
    else
    {
        encoded->encoding_type = 0;
    }

    encoded->target_addressing = target_addressing;
    encoded->opcode = opcode;
    encoded->source_addressing = source_addressing;

    return 1;
}

int is_valid_line_length(const char *line)
{
    if (strlen(line) > LINE_LEN)
    {
        return 0;
    }
    return 1;
}

int is_data(const char *directive)
{
    if (strcmp(directive, directives[0]) == 0)
    {
        return 1;
    }
    else if (strcmp(directive, directives[1]) == 0)
    {
        return 1;
    }

    return 0;
}
int is_valid_data(const char *line)
{
    int i;
    char *directive, *directive_cpy, *values;
    char *curr_val;
    int int_num = 0;

    directive = strchr(line, '.');
    directive++;
    directive_cpy = strdup(directive);
    values = strtok(directive_cpy, " ");
    values = strtok(NULL, " ");
    curr_val = strtok(values, " ");

    while (curr_val != NULL)
    {
        if (is_number(curr_val))
        {
            int_num++;
        }
        curr_val = strtok(NULL, " ");
    }

    return int_num == (strlen(values) / 2);
}

int is_entry(const char *directive)
{
    if (strcmp(directive, ".entry") == 0)
    {
        return 1;
    }
    return 0;
}

int is_extern(const char *directive)
{
    if (strcmp(directive, ".extern") == 0)
    {
        return 1;
    }
    return 0;
}

int is_directive(const char *directive)
{
    int i;
    // Check if directive is a null pointer or an empty string
    if (directive == NULL || directive[0] == '\0')
    {
        return 1;
    }

    // Check if the input directive matches any of the valid directives
    for (i = 0; i < sizeof(directives) / sizeof(directives[0]); i++)
    {
        if (strcmp(directive, directives[i]) == 0)
        {
            return 1;
        }
    }

    return 0;
}
