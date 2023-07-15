#include "../include/line.h"

int is_comment(char *line) {
  char in[MAXLEN];
  strcpy(in, line);

  char *c_pos = strchr(in, ';');
  if (c_pos != NULL && *(c_pos + 1) != '\n' && *(c_pos + 1) != '\t' &&
      *(c_pos + 1) != ' ') {
    return 1;
  }
  return 0;
  
}

void delete_line(char *line) { puts(""); }