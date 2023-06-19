#include "pre_processor.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool is_macro(char *);
Macro get_macro();

int main(int argc, char *argv[]) {
  Macro macros[12];
  int i = 0;
  int arg_index;
  char *token;
  FILE *curr_file;
  char line[MAXLEN];

  for (arg_index = 1; arg_index < argc; arg_index++) {
    curr_file = fopen(argv[arg_index], "r");

    while (fgets(line, 80, curr_file)) {
      if (is_macro(line)) {
        
        i++;
      }
    }

    fclose(curr_file);
  }

  return 0;
}