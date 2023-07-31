#include "first-pass-headers/instruction_helper.h"

/* can be changed to get a command name: i.e. mov */
int num_of_operands(Opcode cmd_opcode /* char *cmd */) {
    /* cmd_opcode = get_opcode(cmd) */
    
    if (cmd_opcode > NULL_OP) {
        if (cmd_opcode < 5) {
            return 2;

        } else if (cmd_opcode < 14) {
            return 1;

        } else {
            if (cmd_opcode <= 15) {
                return 0;
            }
            return -1;
        }
    }
    return -1;
}
