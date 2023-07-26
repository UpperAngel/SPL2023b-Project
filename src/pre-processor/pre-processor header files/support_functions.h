/* Maximum length for a line of content */
#define MAXLEN 100 /* We have chosen an arbitrary length for defining lines to catch line-length errors during the initial pass */
#define MACRO_NAME_LENGTH 31

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Enum to represent different states of parsing a file */
enum MacroState {
    OUTSIDE_MACRO_DEF, /* Indicates that the parser is currently outside a macro definition. */
    INSIDE_MACRO_DEF /* Indicates that the parser is currently inside a macro definition. */
};

/* Struct to represent a line of content */
struct content {
    char line[MAXLEN]; /* The content of the line. */
    struct content *next; /* Pointer to the next line in the linked list of content. */
};

typedef struct content node; /* Typedef for the struct content as node. */

/* Struct to represent a macro */
typedef struct {
    char *name; /* The name of the macro. */
    node *data; /* Pointer to the data (lines of content) of the macro. */
} mcro;

/* Struct to represent a node in the macro table */
struct macroList {
    mcro *macro; /* Pointer to the macro stored in this node. */
    struct macroList *nextMacro; /* Pointer to the next node in the linked list of macros. */
};


/**
 * @brief Adds a new line of content to a given macro.
 *
 * This function appends a new line of content to the specified macro, updating its content list.
 * If the macro is NULL or invalid, the function behavior is undefined.
 *
 * @param[in] macro         Pointer to the target macro to which the line will be added.
 * @param[in] contentSource Pointer to the source content string that will be added to the macro.
 *                          The function copies the content from this source into the macro.
 *
 * @note The function dynamically allocates memory for the new content node, and the caller
 *       is responsible for freeing it and the content list when it's no longer needed.
 *
 * @note If memory allocation for the new content node fails, an appropriate error handling
 *       mechanism should be implemented by the caller.
 *
 * @warning The function assumes that the given macro is a valid pointer to a `mcro` struct, and
 *          that `contentSource` is a valid null-terminated C string.
 */
void add_line_to_mcro(mcro *macro, char *contentSource,int line_number);




/**
 * @brief Creates a new macro with the given name.
 *
 * This function allocates memory for a new `mcro` struct and initializes its name with the provided name.
 * If memory allocation fails, the function returns NULL.
 *
 * @param[in] name  Pointer to a null-terminated C string containing the name of the new macro.
 *                  The function allocates memory to store the name internally.
 *
 * @return A pointer to the newly created macro (`mcro`) with the given name.
 *         If memory allocation fails, the function returns NULL.
 *
 * @note The caller is responsible for freeing the memory of the returned `mcro` struct
 *       and its contents (if applicable) when it's no longer needed.
 *
 * @note If the name is not a valid null-terminated C string, or if memory allocation for the new `mcro`
 *       struct or the name fails, an appropriate error handling mechanism should be implemented by the caller.
 *
 * @warning The function assumes that the given `name` is a valid null-terminated C string.
 *          It is the caller's responsibility to ensure the validity of the input.
 */

struct macroList *createMacroList(int line_number);




/**
 * @brief Creates a new macro with the given name.
 *
 * This function allocates memory for a new `mcro` struct and initializes its name with the provided name.
 * If memory allocation fails, the function returns NULL, indicating an error.
 *
 * @param[in] name  Pointer to a null-terminated C string containing the name of the new macro.
 *                  The function allocates memory to store the name internally.
 *
 * @return A pointer to the newly created macro (`mcro`) with the given name.
 *         If memory allocation fails, the function returns NULL.
 *
 * @note The caller is responsible for freeing the memory of the returned `mcro` struct
 *       and its contents (if applicable) when it's no longer needed.
 *
 * @note If the name is not a valid null-terminated C string, or if memory allocation for the new `mcro`
 *       struct or the name fails, an appropriate error handling mechanism should be implemented by the caller.
 *
 * @warning The function assumes that the given `name` is a valid null-terminated C string.
 *          It is the caller's responsibility to ensure the validity of the input.
 */

mcro *create_mcro(const char *name, int line_number);




/**
 * @brief Adds a macro to the macro list.
 *
 * This function creates a new node in the macro list to store the given macro and appends it to the end of the list.
 * If memory allocation for the new node fails, the function returns without making any changes to the macro list.
 *
 * @param[in] macroToAdd   Pointer to the macro (`mcro`) that needs to be added to the macro list.
 * @param[in,out] macroTablePtr  A double pointer to the first node of the macro list (`struct macroList`).
 *                               The function will update this pointer to the new first node if needed,
 *                               e.g., when the macro list is empty and a new node is created.
 *                               Note that the macroTablePtr itself should not be NULL.
 *
 * @note The caller is responsible for ensuring that the macroToAdd is a valid pointer to a `mcro` struct.
 *       The function does not perform any validation on the macroToAdd pointer.
 *
 * @note The caller is also responsible for managing memory for the macro list and its contents.
 *       This function adds a new node to the list but does not handle deallocation.
 *
 * @warning The function assumes that the macroTablePtr is a valid double pointer to a macro list.
 *          It is the caller's responsibility to ensure the validity of the input.
 *
 * @warning If memory allocation for the new node fails, the function returns without adding the macro to the list.
 *          The caller should check the return value and handle any potential memory allocation errors appropriately.
 *
 * @warning The macroTablePtr itself should not be NULL, and it should point to a valid macro list (even if it's empty).
 */
void add_to_macro_table(mcro *macroToAdd, struct macroList **macroTablePtr, int line_number);




/**
 * @brief Finds a macro in the macro list by its name.
 *
 * This function searches for a macro with the specified name in the macro list and returns a pointer to the
 * corresponding `mcro` structure if found. If either the macroTable pointer or the name pointer is NULL,
 * the function gracefully handles this situation and returns NULL. If a macro with the specified name is not found,
 * the function also returns NULL.
 *
 * @param[in] macroTable   Pointer to the first node of the macro list (`struct macroList`) to search.
 * @param[in] name         The name of the macro to be searched for in the macro list. It should be a null-terminated
 *                         string representing the name of the macro.
 *
 * @return If a macro with the specified name is found in the macro list, the function returns a pointer to the
 *         corresponding `mcro` structure. If the macroTable or name pointer is NULL, or if the macro is not found,
 *         the function returns NULL.
 *
 * @note The function does not perform any validation on the macroTable pointer or the name pointer.
 *       It is the caller's responsibility to ensure that these pointers are valid.
 *
 * @warning The function assumes that the macroTable is a valid pointer to the first node of a macro list.
 *          It is the caller's responsibility to ensure the validity of the macroTable pointer and the macro list.
 *
 * @warning The caller should not modify the contents of the returned `mcro` structure.
 *          If modifications are required, it should be done through appropriate functions provided by the API.
 *
 * @warning The function does not take ownership of the macroTable or the macro list. It does not perform any memory
 *          management for these structures, and the caller is responsible for their memory allocation and deallocation.
 *
 * @see `struct macroList`, `mcro`
 */
mcro *find_macro_by_name(struct macroList *macroTable, const char *name);




/**
 * @brief Checks if the given macro name is valid.
 *
 * This function validates the specified macro name to ensure it meets the criteria for a valid macro name.
 * A macro name is considered valid if it satisfies the following conditions:
 *   1. It is not a reserved keyword.
 *   2. The first character is a letter.
 *   3. The name contains only valid characters, including letters (upper and lowercase) and digits.
 *
 * @param[in] name   The macro name to be validated. It should be a null-terminated string representing the name.
 *
 * @return If the macro name is valid according to the specified conditions, the function returns 1 (true).
 *         Otherwise, if any of the conditions fail, it returns 0 (false).
 *
 * @note The function does not perform any validation on the name pointer.
 *       It is the caller's responsibility to ensure that the name pointer is valid.
 *
 * @warning The function assumes that the name is a valid null-terminated string.
 *          It is the caller's responsibility to ensure the validity and null-termination of the name string.
 *
 * @see `reservedKeywords`
 */
int is_valid_macro_name(const char *name, int line_number);




/**
 * @brief Checks if the given line is a valid macro definition.
 *
 * This function validates the specified line to determine whether it represents a valid macro definition.
 * A valid macro definition follows these rules:
 *   1. The first word must be "mcro".
 *   2. The second word must be a valid macro name (satisfies the conditions defined in is_valid_macro_name function).
 *   3. There should be no more tokens in the line after the macro name (only one macro name allowed in the definition).
 *
 * @param[in] line   The input line to be validated. It should be a null-terminated string representing the line.
 *
 * @return If the line represents a valid macro definition, the function returns 1 (true).
 *         If the line does not start with "mcro", it returns 2 to indicate an invalid macro definition.
 *         If the macro name is not valid or there are additional tokens in the line, it returns 0 (false).
 *
 * @note The function does not perform any validation on the line pointer.
 *       It is the caller's responsibility to ensure that the line pointer is valid.
 *
 * @warning The function assumes that the line is a valid null-terminated string.
 *          It is the caller's responsibility to ensure the validity and null-termination of the line string.
 *
 * @see is_valid_macro_name
 */
int valid_start_macro_def(const char *line, int line_number);



/**
 * @brief Deploys the content of a macro line by line to the specified target file.
 *
 * This function takes a macro and writes its content line by line to the target file.
 * The macro's content is represented by a linked list of nodes, where each node contains a line of the macro content.
 * The lines are written to the target file in the order they appear in the linked list.
 *
 * @param[in] macro        Pointer to the macro whose content needs to be deployed.
 *                         The macro should be a valid pointer to a `mcro` structure.
 * @param[in] target_file  Pointer to the target file where the macro content will be written.
 *                         The target file should be opened in the appropriate mode for writing (e.g., "w").
 *
 * @note The function assumes that the `macro` and `target_file` pointers are valid.
 *       It is the caller's responsibility to ensure the validity of these pointers.
 *
 * @warning The function does not perform any validation on the `macro` or `target_file` pointers.
 *          It is essential to ensure that they are valid and correctly initialized before calling this function.
 *
 * @see mcro
 */
void deploy_macro(const mcro *macro, FILE *target_file);




/**
 * @brief Frees the memory of the macro list and its content nodes.
 *
 * This function deallocates the memory occupied by the macro list and its associated content nodes.
 * The macro list is represented as a linked list of `struct macroList` nodes, and each node contains a pointer to a macro (`mcro`) and a pointer to the next node.
 * For each node in the macro list, this function frees the memory for the macro's name, the linked list of content nodes, and finally, the macro itself.
 * It then proceeds to free the memory for the current node in the macro list before moving to the next node.
 *
 * @param[in] list  Pointer to the first node of the macro list to be freed.
 *                  The list should be a valid pointer to a `struct macroList` node.
 *
 * @note The function assumes that the `list` pointer is valid and points to a correctly initialized macro list.
 *
 * @warning The function does not perform any validation on the `list` pointer.
 *          It is essential to ensure that it is valid and points to a correctly initialized macro list before calling this function.
 *
 * @see mcro, struct macroList
 */
void free_macro_list(struct macroList *list);



/**
 * @brief Frees the memory of the content nodes.
 *
 * This function deallocates the memory occupied by the content nodes of a macro.
 * The content nodes are represented as a linked list of `struct content` nodes, and each node contains a line of content and a pointer to the next node.
 * For each node in the content list, this function frees the memory for the current content node and moves to the next node.
 *
 * @param[in] list  Pointer to the first node of the content list to be freed.
 *                  The list should be a valid pointer to a `struct content` node.
 *
 * @note The function assumes that the `list` pointer is valid and points to a correctly initialized content list.
 *
 * @warning The function does not perform any validation on the `list` pointer.
 *          It is essential to ensure that it is valid and points to a correctly initialized content list before calling this function.
 *
 * @see struct content
 */
void free_content_list(struct content *list);




/**
 * @brief Writes a line to the target file.
 *
 * This function writes the provided line to the specified target file, followed by a newline character '\n'.
 * The line is formatted and printed to the target file using the `fprintf` function from the C standard library.
 *
 * @param[in] line         Pointer to the string representing the line to be written to the file.
 *                         The line should be a valid null-terminated C string.
 *
 * @param[in] target_file  Pointer to the file stream where the line will be written.
 *                         The target_file should be a valid file stream opened for writing using functions like `fopen`.
 *
 * @note The function assumes that both `line` and `target_file` are valid and correctly initialized.
 *
 * @warning The function does not perform any validation on the parameters, and it is the responsibility of the caller to ensure their validity.
 *
 * @see fprintf, fopen
 */
void write_line_to_file(const char *line, FILE *target_file);




/**
 * @brief Determines whether to skip a line based on whether it is a comment or empty.
 *
 * This function checks whether the provided line is a comment or empty, and returns a boolean value accordingly.
 * If the line is a comment (starts with a comment symbol) or empty (contains only whitespace or newline character),
 * the function returns 1 to indicate that the line should be skipped.
 * Otherwise, it returns 0 to indicate that the line should not be skipped and should be processed further.
 *
 * @param[in] line  Pointer to the string representing the line to be checked for skipping.
 *                  The line should be a valid null-terminated C string.
 *
 * @return 1 if the line is a comment or empty, 0 otherwise.
 *
 * @note The function assumes that `line` is a valid and correctly initialized C string.
 *
 * @warning The function does not perform any validation on the parameter, and it is the responsibility of the caller to ensure its validity.
 *
 * @see is_comment, is_empty
 */
int comment_or_empty(char *line);



/**
 * @brief Deploys macros in a line to the target file.
 *
 * This function reads the provided line and identifies tokens separated by whitespace (space, tab, or newline).
 * It then checks if each token matches any of the macro names present in the `macroTable`.
 * If a token corresponds to a macro name, the function deploys the content of that macro (if available) to the `target_file`.
 * If the token does not match any macro name, it is treated as a regular word and written directly to the `target_file`.
 *
 * @param[in] line          The input line containing the text to be processed.
 *                          It should be a valid null-terminated C string.
 * @param[in] target_file   The file pointer of the target file where the processed output will be written.
 *                          The file should be opened in an appropriate mode for writing (e.g., "w").
 * @param[in] macroTable    Pointer to the head of the macro list containing defined macros.
 *                          It should point to a valid `struct macroList` instance that is correctly initialized.
 *
 * @note The function assumes that `line` is a valid and correctly initialized C string.
 * @note The `target_file` should be opened and ready for writing before calling this function.
 * @note The `macroTable` should be a valid pointer to a properly initialized macro list.
 *
 * @warning The function does not perform any validation on the parameters, and it is the responsibility of the caller to ensure their validity.
 *
 * @see deploy_macro, find_macro_by_name
 */
void deploy_macros_in_line(const char line[MAXLEN], FILE *target_file, struct macroList *macroTable);



/**
 * @brief Checks if the line contains a valid macro name.
 *
 * This function takes an input line and checks if the first token in the line matches any of the macro names present in the `macroTable`.
 * If a matching macro name is found, the function returns true (1); otherwise, it returns false (0).
 *
 * @param[in] line          The input line to be checked for a valid macro name.
 *                          It should be a valid null-terminated C string.
 * @param[in] macroTable    Pointer to the head of the macro list containing defined macros.
 *                          It should point to a valid `struct macroList` instance that is correctly initialized.
 *
 * @return An integer representing the result of the check:
 *         - 1 (true) if the first token in the line is a valid macro name and matches a macro in the macro table.
 *         - 0 (false) if the first token in the line does not match any macro name, if the line is empty, or if the macroTable is NULL.
 *
 * @note The function assumes that `line` is a valid and correctly initialized C string.
 * @note The `macroTable` should be a valid pointer to a properly initialized macro list, or it can be NULL to indicate an empty macro list.
 *
 * @warning The function does not perform any validation on the parameters, and it is the responsibility of the caller to ensure their validity.
 *
 * @see find_macro_by_name
 */
int check_line_for_macro(const char line[MAXLEN], struct macroList* macroTable);




/**
 * @brief Removes the newline character at the end of a string, if present.
 *
 * This function takes a C string as input and removes the newline character ('\n') at the end of the string, if it exists. If the last character in the string is a newline character, it is replaced with a null terminator ('\0').
 *
 * @param[in,out] str   The input string from which the newline character will be removed.
 *                      It should be a valid null-terminated C string.
 *
 * @note The function modifies the input string in place.
 * @note If the input string is empty or does not contain a newline character at the end, the function does not perform any changes.
 *
 * @warning The function does not perform any validation on the parameter, and it is the responsibility of the caller to ensure its validity.
 */
void remove_newline(char *str);



/**
 * Checks if a line contains the string "endmcro" with specific conditions.
 *
 * @param line The input line to check for "endmcro".
 * @return 2 if "endmcro" is not found in the line,
 *         1 if "endmcro" is the only content in the line (with or without whitespace characters),
 *         0 if "endmcro" is found in the line with other characters before or after it.
 */
int valid_end_macro_def(const char* line, int line_number);



/**
 * Checks if a given line contains a comment.
 *
 * @param line The input line to check for a comment.
 * @return 1 if the line contains a comment (semicolon followed by non-whitespace character after it),
 *         0 otherwise.
 */
int is_comment(const char *);


/**
 * Checks if a given line is empty (contains only whitespace characters).
 *
 * @param line The input line to check for emptiness.
 * @return 1 if the line is empty (contains only whitespace characters), 0 otherwise.
 */
int is_empty(const char *);



/**
    Extracts the second token from a given input line.
    This function tokenizes the input line using whitespace characters (' ' and '\t') as delimiters
    and returns a pointer to the second token found in the line. It creates a copy of the input line
    to avoid modifying the original string.
    @param line The input line from which the second token will be extracted.
    @return A pointer to the second token if it exists, or NULL if there are less than two tokens in the line.
    */
char* get_second_token (const char *line);



/**
 * Custom function to read a line with a maximum length of MAXLEN from the specified file stream.
 *
 * This function reads characters from the given 'source_file' until 'max_len-1' characters are read,
 * or a newline character is encountered, or the end-of-file (EOF) is reached. The characters are stored
 * in the 'buffer' provided by the caller. The buffer should have enough space to hold at least 'max_len'
 * characters, including the null-terminator.
 *
 * Parameters:
 * - buffer: A pointer to a character array where the read characters will be stored.
 * - max_len: The maximum length of the line to read, including the null-terminator.
 * - source_file: A pointer to the FILE stream from which to read the line.
 *
 * Returns:
 * - On success: The function returns 'buffer', which contains the read line.
 * - On reaching the end-of-file before reading any characters: The function returns NULL.
 *
 * Description:
 * This custom implementation of 'fgets' reads characters from 'source_file' and stores them in 'buffer'
 * until one of the following conditions is met:
 *   - 'max_len-1' characters are read (to accommodate space for the null-terminator).
 *   - A newline character ('\n') is encountered, indicating the end of the line.
 *   - The end-of-file (EOF) is reached.
 * After reading the line, the function adds a null-terminator ('\0') at the end of 'buffer' to mark
 * the end of the string.
 *
 * If the line is longer than 'max_len-1' characters, the function will read the remaining characters
 * from the stream until a newline character or the end of the line is encountered. It discards the
 * exceeding characters to avoid buffer overflow.
 *
 * If the end-of-file is reached without reading any characters, and the 'buffer' is empty (char_count == 0),
 * the function returns NULL to indicate that there is no more data to read.
 *
 * Note:
 * - The 'buffer' should have enough space to hold at least 'max_len' characters (including the null-terminator)
 *   to avoid potential buffer overflows.
 * - The function may return a line shorter than 'max_len-1' characters if a newline character is encountered
 *   before reaching the limit.
 * - The caller should ensure that 'buffer' is a valid pointer and that 'max_len' is a positive integer.
 * - After using the returned line, the caller is responsible for managing the memory of the 'buffer'.
 */
char* custom_fgets(char* buffer, int max_len, FILE* source_file);




/**
 * Check if a given word appears as a complete word in a given line.
 *
 * Parameters:
 * - line: A pointer to a constant C-style string representing the line to be tokenized.
 * - target: A pointer to a constant C-style string representing the word to search for in the tokens.
 *
 * Returns:
 * - 1 if the 'target' word is found as a complete word among the tokens in the 'line'.
 * - 0 otherwise.
 *
 * Description:
 * This function tokenizes the input 'line' using space (' ') and tab ('\t') as delimiters and searches
 * for the 'target' word among the resulting tokens. It checks if the 'target' appears as a complete word,
 * meaning it is a standalone token and not part of a larger word.
 *
 * For example, if the 'target' is "mcro", this function will return 1 for "mcro" but not for "macro" or "micro".
 *
 * Note:
 * - The function works with a copy of the 'line' since 'strtok' modifies the input string.
 *   The caller should provide an appropriate buffer size for 'MAXLEN' when calling this function.
 * - The 'line' should be a valid C-style string (null-terminated).
 * - The 'target' should be a valid C-style string (null-terminated).
 * - The line should not exceed the maximum token length that can be handled by the function.
 */
int find_word_in_tokens(const char* line, const char* target);




/**
 * Check if the given line contains the beginning of a macro definition.
 *
 * Parameters:
 * - line: A pointer to a constant C-style string representing the line to be checked.
 *
 * Returns:
 * - 1 if the word "mcro" is found among the tokens in the line.
 * - 0 otherwise.
 *
 * Description:
 * This function checks if the given 'line' contains the word "mcro" as one of its tokens.
 * It does this by tokenizing the line using space (' ') and tab ('\t') as delimiters and
 * then searching for the word "mcro" among the resulting tokens.
 *
 * Note:
 * - The function does not consider the context of the word "mcro" and only checks if it exists
 *   as a separate token within the line.
 * - The line should be a valid C-style string (null-terminated).
 * - The line should not exceed the maximum token length that can be handled by 'find_word_in_tokens'.
 */
int is_start_macro_def(const char* line);


/**
 * Check if the given line contains the ending of a macro definition.
 *
 * Parameters:
 * - line: A pointer to a constant C-style string representing the line to be checked.
 *
 * Returns:
 * - 1 if the word "endmcro" is found among the tokens in the line.
 * - 0 otherwise.
 *
 * Description:
 * This function checks if the given 'line' contains the word "endmcro" as one of its tokens.
 * It does this by tokenizing the line using space (' ') and tab ('\t') as delimiters and
 * then searching for the word "endmcro" among the resulting tokens.
 *
 * Note:
 * - The function does not consider the context of the word "endmcro" and only checks if it exists
 *   as a separate token within the line.
 * - The line should be a valid C-style string (null-terminated).
 * - The line should not exceed the maximum token length that can be handled by 'find_word_in_tokens'.
 */
int is_end_macro_def(const char* line);




















