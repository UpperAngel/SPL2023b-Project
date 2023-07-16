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

int is_endmcro(const char *line)
{
  char *word;
  char *mod_line = malloc(strlen(line) + 1);
  strcpy(mod_line, line);

  word = strtok(mod_line, " ");
  if (!strcmp(word, "endmcro"))
  {
    return 1;
  }
  return 0;
}

is_empty(const char *line)
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

void delete_line(char *line) { puts(""); }