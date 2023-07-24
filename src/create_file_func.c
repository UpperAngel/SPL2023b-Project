#include "pre-processor/pre-processor header files/pre_processor.h"

FILE *create_file(const char *file_name, const char *extension) {
  FILE *created_file = NULL;
  
  char *new_file_name;
  char *dot = NULL;

  /* dynamiclly allocate memory for the file name and the extension */
  new_file_name = (char *)malloc(strlen(file_name) + 1);
  if (new_file_name == NULL)
  {
    printf("faild allocation for the file name \n");
    return NULL;
  }
  
  /* copy the input to file_name */
  strcpy(new_file_name, file_name);
  
  /* deletes the file extension. everything after the dot and the dot itself */
  dot = strchr(new_file_name, '.');
  if (dot != NULL)
  {
    *dot = '\0';
  }
  /* adds the extension and creates the file*/
  new_file_name = strcat(new_file_name, extension);
  created_file = fopen(new_file_name, "w+");

  /* checks if file was created succesfully */
  if (created_file == NULL)
  {
    printf("failed to create file %s \n", new_file_name);
    free(new_file_name);
    return NULL;
  }

  /* closes the file and returns it*/
  fclose(created_file);
  return created_file;
}