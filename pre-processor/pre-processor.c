#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLEN 80

int main(int argc, char *argv[]) {
  int arg_index;

  FILE *curr_file;
  char line[MAXLEN];

  for (arg_index = 1; arg_index < argc; arg_index++) {
    curr_file = fopen(argv[arg_index], "r");

    while (fgets(line, 80, curr_file)) {
       
    }

    fclose(curr_file);
  }

  return 0;
}