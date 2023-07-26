
#ifndef SPL2023B_PROJECT_COPY_HANDLE_ERROR_H
#define SPL2023B_PROJECT_COPY_HANDLE_ERROR_H

#endif /* SPL2023B_PROJECT_COPY_HANDLE_ERROR_H */

enum Error {
    NoError,
    ExcessCharactersAtStartMacro,
    ExcessCharactersAtEndMacro
};

void handle_error(enum Error error, int line_number);
