#ifndef __line_h
#define __line_h

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLEN 80

bool is_comment(char *);
void delete_line(char *);

#endif