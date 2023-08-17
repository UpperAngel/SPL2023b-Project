#include "/home/david/CLionProjects/Project main branch/src/FIRST ---PASS/header files/first_pass_headers.h"
int is_whitespace(char c) {
    return c == ' ' || c == '\t';
}
void format_line(char source[LEN], char formatted_line[LEN]) {
    /* Get the length of the input line */
    int i = 0;
    int len = (int)strlen(source);

    /* Variables to keep track of the formatted line */
    int formatted_index = 0;
    int last_was_space = 1;

    /* Iterate through each character of the input line */
    for (i; i < len; i++) {
        char current_char = source[i];
        int is_space = is_whitespace(current_char);

        /* Handle spaces and tabs */
        if (is_space) {
            if (!last_was_space && formatted_index > 0 && formatted_line[formatted_index - 1] != ',') {
                formatted_line[formatted_index++] = ' '; /* Replace multiple spaces with a single space */
            }
            last_was_space = 1;
        }
            /* Handle commas */
        else if (current_char == ',') {
            if (!last_was_space) {
                if (formatted_index > 0 && formatted_line[formatted_index - 1] != ',') {
                    formatted_line[formatted_index++] = ' '; /* Add a space before comma if there wasn't one */
                }
            }

            formatted_line[formatted_index++] = current_char;
            formatted_line[formatted_index++] = ' '; /* Add a space after comma */
            last_was_space = 1;
        }
            /* Handle regular characters */
        else {
            formatted_line[formatted_index++] = current_char;
            last_was_space = 0;
        }
    }

    /* Remove trailing whitespace, if any */
    if (formatted_index > 0 && is_whitespace(formatted_line[formatted_index - 1])) {
        formatted_index--;
    }

    /* Null-terminate the formatted string */
    formatted_line[formatted_index] = '\0';
}
void store_words(char line[], char resultArray[LEN][LEN]) {
    char* token;
    const char delimiter[] = " ";
    int index = 0;

    memset(resultArray, 0, sizeof(resultArray[0][0]) * LEN * LEN);

    token = strtok(line, delimiter);

    while (token != NULL && index < LEN - 1) {
        if (strlen(token) < LEN) {
            strncpy(resultArray[index], token, LEN - 1);
            resultArray[index][LEN - 1] = '\0';
            index++;
        }
        token = strtok(NULL, delimiter);
    }
}
void format_and_store_words(char line[LEN],char words_array[LEN][LEN]){
    char formatted_line[LEN] =  {0};
    format_line(line,formatted_line);
    store_words(formatted_line,words_array);
}
void update_variables(char **current_symbol_name, int *symbol_definition, int *line_number, int *index){
    *(current_symbol_name) = NULL;
    *symbol_definition = 0;
    *line_number = *line_number + 1;
    *index = 0;
}
int is_comment(const char* line) {
    int index = 0;

    while (isspace(line[index])) {
        index++; /* Skip leading whitespace characters. */
    }

    return (line[index] == ';') ? 1 : 0;
}

/* Function to check if a given line is empty (contains only whitespace characters) */
int is_empty(const char *line) {
    while (*line != '\0') {
        /* Loop through each character in the line until the end of the string is reached. */

        if (!isspace((unsigned char)*line)) {
            /*
             * Check if the current character is not a whitespace character.
             * The `(unsigned char)` cast is used to handle potential negative char values correctly.
             */

            return 0; /* Return 0 immediately if a non-whitespace character is found. */
        }

        line++; /* Move to the next character in the line. */
    }

    return 1; /* Return 1 if the loop reaches the end of the line without finding a non-whitespace character. */
}

/* Function to skip a line if it is a comment or empty */
int comment_or_empty(char *line)
{
    return ( is_comment(line) || is_empty(line));
}
