
#ifndef SPL2023B_PROJECT_COPY_HANDLE_ERROR_H
#define SPL2023B_PROJECT_COPY_HANDLE_ERROR_H

#endif /* SPL2023B_PROJECT_COPY_HANDLE_ERROR_H */

/**
 * Enumeration of error codes for the macro processing.
 *
 * The 'enum Error' defines various error codes that can occur during the macro processing.
 * Each error code represents a specific type of error encountered during the processing
 * of macro definitions.

 * The 'enum Error' provides a convenient way to categorize and identify different error scenarios
 * encountered while processing macros. It can be used in conjunction with the 'handle_error' function
 * to print meaningful error messages and perform appropriate error handling during macro processing.
 *
 */
enum Error {
    NoError,
    ExcessCharactersAtStartMacro,
    ExcessCharactersAtEndMacro,
    InvalidMacroName,
    InvalidCharacterInName,
    FirstCharacterNotLetter,
    ReservedKeywordName,
    NameExceedsMaxLength,
    FailedToAllocateMemory
};



/**
 * Function to handle errors and print the error message to the standard error stream (stderr).
 *
 * Parameters:
 * - error: An enum Error representing the specific error that occurred.
 * - line_number: An integer representing the line number where the error occurred.
 *
 * Description:
 * This function takes an 'error' code and a 'line_number' as input and prints an appropriate error
 * message to the standard error stream (stderr) based on the specific 'error' that occurred. It provides
 * descriptive error messages for different error cases encountered during the macro processing.
 *
 * The 'error' parameter should be one of the following values:
 * - NoError: Indicates that there is no error and the function will do nothing.
 * - ExcessCharactersAtStartMacro: Indicates that there are excess characters at the start of a macro definition.
 * - ExcessCharactersAtEndMacro: Indicates that there are excess characters at the end of a macro definition.
 * - InvalidMacroName: Indicates that the macro name is invalid.
 * - InvalidCharacterInName: Indicates that the macro name contains an invalid character.
 * - FirstCharacterNotLetter: Indicates that the first character of the macro name is not a letter.
 * - ReservedKeywordName: Indicates that the macro name is a reserved keyword.
 * - NameExceedsMaxLength: Indicates that the macro name exceeds the maximum allowed length.
 * - FailedToAllocateMemory: Indicates a failure to allocate memory during the macro processing.
 *
 * The 'line_number' parameter should be the line number in the source file where the error was encountered.
 *
 * Note:
 * - The function prints the error messages to the standard error stream (stderr), which is typically used
 *   for displaying error messages and diagnostics, separate from the standard output (stdout).
 * - The function should be used in error-handling scenarios within the macro processing to provide meaningful
 *   error messages to the user or developer.
 */
void handle_error(enum Error error, int line_number);
