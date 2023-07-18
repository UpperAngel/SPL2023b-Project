#ifndef __line_h
#define __line_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAXLEN 80

int is_comment(const char *);
void delete_line(char *);
int is_endmcro(const char *);
int is_empty(const char *);

#endif
