#ifndef __ERRORS_H
#define __ERRORS_H
#include <stdio.h>

enum Error;

extern const char *errorMessages[];

void handle_error(enum Error error, int line_number);

#endif
