#include "handle_error.h"
#include <stdio.h>

/* Function to handle errors and print the error message to stderr */
void handle_error(enum Error error, int line_number) {
    switch (error) {
        case NoError:
            /* Do nothing when there is no error */
            break;
        case ExcessCharactersAtStartMacro:
            fprintf(stderr, "Error in line %d: Excess characters at the start of macro definition\n", line_number);
            break;
        case ExcessCharactersAtEndMacro:
            fprintf(stderr, "Error in line %d: Excess characters at the end of macro definition\n", line_number);
            break;
        case InvalidMacroName:
            fprintf(stderr, "Error in line %d: Invalid macro name\n", line_number);
            break;
        case InvalidCharacterInName: /* New case for "Name contains an invalid character" error */
            fprintf(stderr, "Error in line %d: Name contains an invalid character\n", line_number);
            break;
        case FirstCharacterNotLetter: /* New case for "First character is not a letter" error */
            fprintf(stderr, "Error in line %d: First character is not a letter\n", line_number);
            break;
        case ReservedKeywordName: /* New case for "Name is a reserved keyword" error */
            fprintf(stderr, "Error in line %d: Name is a reserved keyword\n", line_number);
            break;
        case NameExceedsMaxLength: /* New case for "Name exceeds the maximum allowed length" error */
            fprintf(stderr, "Error in line %d: Name exceeds the maximum allowed length\n", line_number);
            break;
        case FailedToAllocateMemory: /* New case for "Failed to allocate memory" error */
            fprintf(stderr, "Error in line %d: Failed to allocate memory\n", line_number);
            break;
        default:
            fprintf(stderr, "Unknown error occurred\n");
            break;
    }
}


