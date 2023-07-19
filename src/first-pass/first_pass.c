#include "../include/first_pass.h"
#include "../include/utils.h"

enum opcodes
{
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

struct cm_entry
{
    const char *command;
    opcodes opcode;
};

struct instruction
{
    const char *command;
    const char *source_oparend;
    const char *target_oparend;
}

commandmap command_map[] = {
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

opcodes get_command_opcode(char *command)
{
    int i;

    if (command == NULL) /* if the command is null we return the null op*/
    {
        return null_op;
    }
    for (i = 0; i < command_map[i].command != NULL; i++)
    {
        if (strcmp(command, command_map[i].command) == 0)
        {
            return command_map[i].opcode;
        }
    }
    return null_op;
}

BOOL is_valid_label( const char label[31])
{
    
    int i;
    if (!isalpha(label[0]))
    {
        return FALSE;
    }
    
    /* checks command names */
    for (i = 0; i < command_map[i].command != NULL; j++)
    {
        if (!isalnum(label[i]))
        {
            return FALSE;
        }
        
        else if (strcmp(label, command_map[i].command) == 0)
        {
            return FALSE;
        }

        /* add check against label database */       
    }
    return TRUE;
        
}