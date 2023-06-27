#include "../include/line.h"

bool is_comment(char *line) {
  char *in;
  strcpy(in, line);

  char *c_pos = strchr(in, ';');
  if (c_pos != NULL && *(c_pos + 1) != '\n' && *(c_pos + 1) != '\t' &&
      *(c_pos + 1) != ' ') {
    return true;
  }
  return false;
}

void delete_line(char *line) { puts(""); }