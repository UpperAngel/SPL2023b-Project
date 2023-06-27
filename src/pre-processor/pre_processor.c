#include "../include/pre_processor.h"


bool is_empty();
void add_line(char *);

void process_file(FILE *file) {
  int is_macro = 0;
  int i = 0;
  int arg_index;
  char line[MAXLEN];

  while (fgets(line, 80, file) != NULL) {
    if (strstr(line, "mcro")) {
      is_macro = 1;
      delete_line(line);
    } else if (strstr(line, "endmcro")) {
      is_macro = 0;
      delete_line(line);
    } else {
      /* inside macro */
      add_line(line);
      delete_line(line);
    }

    if (is_comment(line)) {
      delete_line(line);
    } else if (is_empty()) {
      delete_line(line);
    }

    fprintf_s(file, "%s", line);
    
  } 
}