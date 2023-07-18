#include "../include/line.h"

int is_comment(const char *line)
{
  char in[MAXLEN];
  strcpy(in, line);

  char *c_pos = strchr(in, ';');
  if (c_pos != NULL && *(c_pos + 1) != '\n' && *(c_pos + 1) != '\t' &&
      *(c_pos + 1) != ' ')
  {
    return 1;
  }
  return 0;
}

int is_endmcro(const char *line) {
    // Allocate memory for mod_line and check if malloc was successful
    char *mod_line = malloc(strlen(line) + 1);
    if (mod_line == NULL) {
        // Memory allocation failed; handle the error appropriately
        return 0;
    }

    // Copy the line into mod_line before calling strtok
    strcpy(mod_line, line);

    // Use strtok to tokenize the line
    char *word = strtok(mod_line, " ");

    int result = (word != NULL && strcmp(word, "endmcro") == 0);

    free(mod_line); // Free the dynamically allocated memory
    return result;
}




int is_empty(const char *line)
{
  while (*line != '\0')
  {
    if (!isspace((unsigned char)*line))
    {
      return 0;
    }
    line++;
  }
  return 1;
}



