#include "handle_error.h"
#include <stdio.h>

/* Function to handle errors and print the error message to stderr */
void handle_error(enum Error error, int line_number) {
    switch (error) {
        case NoError:
            // Do nothing when there is no error
            break;
        case ExcessCharactersAtStartMacro:
            fprintf(stderr, "Error in line %d: Excess characters at the start of macro definition\n", line_number);
            break;
        case ExcessCharactersAtEndMacro:
            fprintf(stderr, "Error in line %d: Excess characters at the end of macro definition\n", line_number);
            break;
        default:
            fprintf(stderr, "Unknown error occurred\n");
            break;
    }
}


