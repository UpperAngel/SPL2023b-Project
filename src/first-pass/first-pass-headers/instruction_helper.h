#ifndef __INSTRUCTION_HELPER_H
#define __INSTRUCTION_HELPER_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <symbol_table.h>
#include <instruction.h>
#include <first_pass.h>

#define LEN 82
/**
 * gets the number of operands a command is expacted to have.
 * @param cmd_opcode the opcode of the command we get the number of operands.
 * @return 0,1 or 2 in relation to the opcode. if it is an unknown we return -1.  
*/
int num_of_operands(Opcode cmd_opcode /* char *cmd */);
int is_register(const char *operand);
#endif