#include "errors.h"

void handle_error(enum Error error, int line_number) {
    if (error != NoError) {
        fprintf(stderr, "Error in line %d: %s\n", line_number, errorMessages[error]);
    }
}