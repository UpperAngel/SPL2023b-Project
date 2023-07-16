#include "../include/pre_processor.h"

void process_file(FILE *file, FILE *target_file)
{

  int i;
  int is_macro = 0;
  char line[MAXLEN];
  char *curr_mcro_name;
  mcro *created_macro;
  mcro *curr_macro;
  struct macroList *macro_list = createMacroList();

  while (fgets(line, 80, file) != NULL)
  {
    if (is_valid_macro_def(line))
    {
      is_macro = 1;
      curr_mcro_name = strtok(NULL, " ");
      created_macro = create_mcro(curr_mcro_name);
      add_to_macro_table(created_macro, macro_list);
    }

    if (is_endmcro)
    {
      is_macro = 0;
      delete_line(line);
    }

    if (is_comment)
    {
      delete_line(line);
    }

    if (is_empty(line))
    {
      delete_line(line);
    }
    if (is_macro == 1)
    {
      add_line_to_mcro(created_macro, line);
    }

    if ((curr_macro = find_macro_by_name(macro_list, line)))
    {
      deploy_macro(curr_macro, target_file);
    }
    

    fprintf_s(target_file, "%s", line);
  }
}