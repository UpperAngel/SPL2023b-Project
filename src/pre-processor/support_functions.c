#include "pre-processor header files/pre_processor.h"
#include "handle_error.h"

/* API style documentation of every function is in the "support_function.h" file */

/* Function to add a line to the content of a macro */
void add_line_to_mcro(mcro *macro, char *contentSource, int line_number)
{
    /* Pointer to the pointer to the data member */
    struct content **ptr = &(macro->data);

    /* Create a new node */
    struct content *macroNode = (struct content *)malloc(sizeof(struct content));

    if(macroNode == NULL){
        handle_error(FailedToAllocateMemory, line_number);
        exit(0);
    }


    /* Copy the content into the new node */
    strcpy(macroNode->line, contentSource);
    macroNode->next = NULL;

    /* If the data member is NULL, update it with the new node */
    if (*ptr == NULL)
    {
        *ptr = macroNode;
    }
        /* If the data member is not NULL, find the last node and append the new node */
    else
    {
        while ((*ptr)->next != NULL)
        {
            ptr = &((*ptr)->next);
        }
        (*ptr)->next = macroNode;
    }
}




/* Function to add a macro to the macro list */
void add_to_macro_table(mcro *macroToAdd, struct macroList **macroTablePtr, int line_number)
{
    /* Create a new node */
    struct macroList *newNode = malloc(sizeof(struct macroList));
    if (newNode == NULL) {
        handle_error(FailedToAllocateMemory, line_number);
        exit(0);
    }


    newNode->macro = macroToAdd;
    newNode->nextMacro = NULL;

    /* If the macro table is empty */
    if ((*macroTablePtr)->macro == NULL)
    {
        *macroTablePtr = newNode;
    }
    else
    {
        struct macroList *ptr = *macroTablePtr;

        /* Traverse to the end of the macro table */
        while (ptr->nextMacro != NULL)
        {
            ptr = ptr->nextMacro;
        }

        /* Append the new node to the end of the macro table */
        ptr->nextMacro = newNode;
    }
}




/* Function to create a new macro with the given name */
mcro *create_mcro(const char *name, int line_number)
{
    mcro *newMcro = malloc(sizeof(mcro));
    if (newMcro == NULL) {
        handle_error(FailedToAllocateMemory, line_number);
        exit(0);
    }

    if (newMcro != NULL)
    {
        newMcro->name = strdup(name); /* Allocate memory and copy the name */
        newMcro->data = NULL;         /* Initialize data member to NULL */
    }

    return newMcro;
}




/* Function to create a new macro list */
struct macroList *createMacroList(int line_number)
{
    struct macroList *newList = malloc(sizeof(struct macroList));
    if (newList == NULL) {
        handle_error(FailedToAllocateMemory, line_number);
        exit(0);
    }

    if (newList != NULL)
    {
        newList->macro = NULL;     /* Initialize macro member to NULL */
        newList->nextMacro = NULL; /* Initialize nextMacro member to NULL */
    }

    return newList;
}




/* Function to find a macro in the macro list by its name */
mcro *find_macro_by_name(struct macroList *macroTable, const char *name)
{
    struct macroList *current;

    if (macroTable == NULL || name == NULL)
    {
        /* Handle the case of null pointers */
        return NULL;
    }

    current = macroTable;

    while (current != NULL)
    {
        mcro *currentMacro = current->macro;

        if (currentMacro == NULL || currentMacro->name == NULL)
        {
            /* Skip this node as it contains invalid data */
            current = current->nextMacro;
            continue;
        }

        if (strcmp(currentMacro->name, name) == 0)
        {
            return currentMacro; /* Found matching macro, return pointer to it */
        }

        current = current->nextMacro; /* Move to the next node in the macroList */
    }

    return NULL; /* Macro not found, return NULL */
}





#include <stdio.h>
#include <string.h>
#include <ctype.h>

int is_valid_macro_name(const char *name, int line_number) {
    /* Maximum allowed length for macro name */
    const int maxNameLength = MACRO_NAME_LENGTH;
    int i = 0;
    size_t j = 0;
    int numReservedKeywords = 0;

    /* Check if name is a reserved keyword */
    const char *reservedKeywords[] = {
            "mov", "cmp", "add", "sub", "not", "clr", "lea", "inc",
            "de", "jmp", "bne", "red", "prn", "jsr", "rts", "stop"
    };

    /* Check the length of the name */
    size_t nameLength = strnlen(name, maxNameLength + 1);

    /* Check if the length exceeds the maximum allowed length */
    if (nameLength > maxNameLength) {
        handle_error(NameExceedsMaxLength,line_number);
        return 0;
    }


     numReservedKeywords = sizeof(reservedKeywords) / sizeof(reservedKeywords[0]);

    for (i = 0; i < numReservedKeywords; i++) {
        if (strcmp(name, reservedKeywords[i]) == 0) {
            handle_error(ReservedKeywordName,line_number); /* Name is a reserved keyword */
            return 0;
        }
    }

    /* Check if the first character is a letter */
    if (!isalpha(name[0])) {
        handle_error(FirstCharacterNotLetter,line_number); /* First character is not a letter */
        return 0;
    }

    /* Check if the name contains only valid characters (letters and digits) */
    for (j = 0; j < nameLength; j++) {
        if (!isalnum(name[j])) {
            handle_error(InvalidCharacterInName,line_number);
            return 0; /* Name contains an invalid character */
        }
    }

    /* Name passes all checks */
    return 1;
}





/* Function to check if the line is a valid macro definition */
int valid_start_macro_def(const char* line, int line_number) {
    char* token = NULL;
    const char* mcro_str = "mcro";

    /* Allocate memory for mod_line and check if malloc was successful */
    char* mod_line = malloc(strlen(line) + 1);
    if (mod_line == NULL) {
        handle_error(FailedToAllocateMemory, line_number);
        return 0;
    }

    /* Copy the line into mod_line before calling strtok */
    strcpy(mod_line, line);

    /* Use strtok to tokenize the line */
    token = strtok(mod_line, " \t");

    /* Check if the first token is "mcro" */
    if (token == NULL || strcmp(token, mcro_str) != 0) {
        free(mod_line);
        handle_error(ExcessCharactersAtStartMacro,line_number);
        return 0;
    }

    /* Get the next token (macro name) */
    token = strtok(NULL, " \t");
    if (token == NULL || !is_valid_macro_name(token, line_number)) {
        /* Either the token is NULL or the macro name is invalid */
        free(mod_line);
        return 0;
    }

    /* Check if there are any more tokens (should be no more tokens for a valid macro definition) */
    token = strtok(NULL, " ");
    if (token != NULL) {
        /* There are more tokens, which is not allowed in a valid macro definition */
        handle_error(ExcessCharactersAtStartMacro,line_number);
        free(mod_line);
        return 0;
    }

    /* If all conditions are satisfied, the macro definition is valid */
    free(mod_line);
    return 1;
}



/* Function to check if the line contains "endmcro" with or without other words */
int valid_end_macro_def(const char *line, int line_number) {
    char *first_word;
    char *second_word;
    char *target = "endmcro";

    /* Allocate memory for mod_line and check if malloc was successful */
    char *mod_line = malloc(strlen(line) + 1);
    if (mod_line == NULL) {
        handle_error(FailedToAllocateMemory,line_number);
        return 0;
    }

    /* Copy the line into mod_line before calling strtok */
    strcpy(mod_line, line);

    /* Use strtok to tokenize the line */
    first_word = strtok(mod_line, " \t");
    second_word = strtok(NULL, " \t");

    if (strcmp(target, first_word) == 0 && second_word == NULL)
        return 1; /* "endmcro" is the only word in the line. */

    free(mod_line); /* Free the dynamically allocated memory. */

    handle_error(ExcessCharactersAtEndMacro,line_number);
    return 0; /* "endmcro" found with other words in the line. */
}






/* Function to deploy the content of a macro line by line to the target file */
void deploy_macro(const mcro *macro, FILE *target_file)
{
    /* Deploy its content line by line to the target_file */
    node *currentNode = macro->data;
    while (currentNode != NULL)
    {
        fprintf(target_file, "%s\n", currentNode->line);
        currentNode = currentNode->next;
    }
}




/* Function to free the memory of the content nodes */
void free_content_list(struct content *list) {
    while (list != NULL) {
        struct content *temp = list; /* Store the current node in a temporary variable */
        list = list->next; /* Move to the next node before deallocating the current one */

        /* Free the memory for the current content node */
        free(temp);
    }
}




/* Function to free the memory of the macro list and its content nodes */
void free_macro_list(struct macroList *list) {
    while (list != NULL) {
        struct macroList *temp = list; /* Store the current node in a temporary variable */
        list = list->nextMacro; /* Move to the next node before deallocating the current one */

        /* Make sure the macro is not NULL before freeing its memory */
        if (temp->macro != NULL) {
            /* Free the memory for the macro name and the linked list of content nodes */
            free(temp->macro->name);
            free_content_list(temp->macro->data);

            /* Free the memory for the macro itself */
            free(temp->macro);
        }

        /* Free the memory for the current node in the macro list */
        free(temp);
    }
}




/* Function to write a line to the target file */
void write_line_to_file(const char *line, FILE *target_file) {
    fprintf(target_file, "%s\n", line);
}




/* Function to skip a line if it is a comment or empty */
int comment_or_empty(char *line)
{
    return ( is_comment(line) || is_empty(line));
}




/* Function to deploy macros in a line to the target file */
void deploy_macros_in_line(const char line[MAXLEN], FILE *target_file, struct macroList *macroTable) {
    char copyLine[MAXLEN];
    char *tokens [MAXLEN]; /* Array to store the tokens */
    int numTokens = 0;
    int i = 0;
    char *token = NULL;

    strcpy(copyLine, line);

    /* Break the line into tokens */
    token = strtok(copyLine, " \t");
    while (token != NULL) {
        tokens[numTokens++] = token;
        token = strtok(NULL, " \t");
    }

    /* Iterate through the tokens and check if they match any macro name */
    for (i = 0; i < numTokens; i++) {
        mcro *foundMacro = find_macro_by_name(macroTable, tokens[i]);
        if (foundMacro != NULL) {
            /* Deploy the macro's lines to the target file */
            deploy_macro(foundMacro, target_file);
        } else {
            /* Write the token to the target file if it is not a macro name */
            fprintf(target_file, "%s ", tokens[i]);
        }
    }
}




/* Modify the check_line_for_macro function to return a boolean */
int check_line_for_macro(const char line[MAXLEN], struct macroList* macroTable) {
    char copyLine[MAXLEN];
    /* Array to store the tokens */
    char* tokens[MAXLEN];
    int numTokens = 0;
    char* token;

    strcpy(copyLine, line);

    /* Break the line into tokens */
    token = strtok(copyLine, " \t\n");
    while (token != NULL) {
        tokens[numTokens++] = token;
        token = strtok(NULL, " \t\n");
    }

    /* Check if the first token is a macro name */
    if (numTokens > 0) {
        mcro* foundMacro = find_macro_by_name(macroTable, tokens[0]);
        return (foundMacro != NULL);
    }

    /* If there is no macro with the same name, return false (0) */
    return 0;
}




/* Function to remove newline character at the end of a string */
void remove_newline(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n')
        str[len - 1] = '\0';
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

/* Function that gets the second word from a given line */
char* get_second_token (const char* line){
    /* Create a copy of the input line to avoid modifying the original string */
    char* line_copy = strdup(line);

    /* Initialize variables to keep track of tokens and the second token */
    char* token;
    char* second_token = NULL;
    int token_count = 0;

    /* Tokenize the line using " \t" as the delimiter */
    token = strtok(line_copy, " \t");

    /* Loop through all tokens and find the second one */
    while (token != NULL) {
        token_count++;
        if (token_count == 2) {
            /* Store the second token */
            second_token = strdup(token);
            break;
        }
        token = strtok(NULL, " \t");
    }

    /* Free the memory allocated for the copy of the line */
    free(line_copy);

    /* Return the second token */
    return second_token;
}




/* Custom function to read a line with a maximum length of max_len */
char* custom_fgets(char* buffer, int max_len, FILE* source_file) {
    int current_char;
    int char_count = 0;

    /* Read characters until max_len-1 or newline or EOF is encountered */
    while (char_count < max_len - 1 && (current_char = fgetc(source_file)) != EOF && current_char != '\n') {
        buffer[char_count++] = (char)current_char;

    }

    buffer[char_count] = '\0'; /* Add the null-terminator to mark the end of the string */

    /* If the line is too long, skip the rest of the characters until a newline or the end of the line is encountered */
    if (current_char != '\n' && current_char != EOF) {
        while ((current_char = fgetc(source_file)) != '\n' && current_char != EOF); /* Skip the rest of the line */
    }

    return (current_char == EOF && char_count == 0) ? NULL : buffer;
}


/* Function to check if a given word appears as a complete word in a given line. */
int find_word_in_tokens(const char* line, const char* target) {
    const char* delimiters = " \t";
    char line_copy[MAXLEN]; /* A copy of the line to work with, as strtok modifies the input */
    char* token;

    /* Create a copy of the line as strtok modifies the input string */
    strncpy(line_copy, line, MAXLEN);

    /* Use strtok to tokenize the line */
    token = strtok(line_copy, delimiters);

    while (token != NULL) {
        if (strcmp(token, target) == 0) {
            return 1; /* Found the target word in the tokens */
        }

        token = strtok(NULL, delimiters);
    }

    return 0; /* Target word not found in the tokens */
}



int is_start_macro_def(const char* line) {
    return find_word_in_tokens(line, "mcro");
}

int is_end_macro_def(const char* line) {
    return find_word_in_tokens(line, "endmcro");
}



















