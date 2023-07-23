#ifndef __PRE_PROCESSOR_H

#define __PRE_PROCESSOR_H


#include "support_functions.h"

/**
 * @brief Process the source file and extract macros, writing the processed content to the target file.
 *
 * This function reads the source file line by line, processes each line to identify macros and their content, and writes the processed content to the target file. The source file and target file are specified by their respective FILE pointers.
 *
 * @param[in] source_file    A pointer to the source file to be processed.
 *                           It should be opened in read mode ('r') before passing it to the function.
 * @param[in,out] target_file   A pointer to the target file where the processed content will be written.
 *                              It should be opened in write mode ('w') before passing it to the function.
 *
 * @return 1 if the target file was built successfully, 0 otherwise.
 *
 * @note The function assumes that both source_file and target_file are valid FILE pointers.
 * @note The source file should be opened before calling this function, and the target file will be modified during the process.
 * @note The function handles macro definition lines, macro expansion in the source file, and writes the processed content to the target file.
 * @note If there is an invalid macro definition in the source file or a memory allocation failure, the function will return 0, indicating that the target file was not built successfully.
 * @note Before calling this function, ensure that the macroList structure and the necessary helper functions (e.g., add_line_to_mcro, create_mcro, add_to_macro_table, etc.) have been correctly defined and implemented.
 * @note The function frees the dynamically allocated memory for the macro list before exiting.
 */

int process_file(FILE *,FILE*);

#endif
