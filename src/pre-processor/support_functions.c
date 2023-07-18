#include "../include/support_functions.h"
/* API style documentation of every function is in the "supportFunction.h" file */

void add_line_to_mcro(mcro *macro, char *contentSource)
{
    /* Pointer to the pointer to the data member */
    struct content **ptr = &(macro->data);

    /* Create a new node */
    struct content *macroNode = (struct content *)malloc(sizeof(struct content));

    /********************HERE SHOULD BE ADDED ERROR IF FAILED TO ALLOCATE MEMORY *********/

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

void add_to_macro_table(mcro *macroToAdd, struct macroList **macroTablePtr)
{
    /* Create a new node */
    struct macroList *newNode = malloc(sizeof(struct macroList));
    /********************HERE SHOULD BE ADDED ERROR IF FAILED TO ALLOCATE MEMORY *********/

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

mcro *create_mcro(const char *name)
{
    mcro *newMcro = malloc(sizeof(mcro));
    /********************HERE SHOULD BE ADDED ERROR IF FAILED TO ALLOCATE MEMORY *********/

    if (newMcro != NULL)
    {
        newMcro->name = strdup(name); /* Allocate memory and copy the name */
        newMcro->data = NULL;         /* Initialize data member to NULL */
    }

    return newMcro;
}

struct macroList *createMacroList()
{
    struct macroList *newList = malloc(sizeof(struct macroList));
    /********************HERE SHOULD BE ADDED ERROR IF FAILED TO ALLOCATE MEMORY *********/

    if (newList != NULL)
    {
        newList->macro = NULL;     /* Initialize macro member to NULL */
        newList->nextMacro = NULL; /* Initialize nextMacro member to NULL */
    }

    return newList;
}

mcro *find_macro_by_name(struct macroList *macroTable, const char *name)
{
    if (macroTable == NULL || name == NULL)
    {
        /* Handle the case of null pointers gracefully */
        return NULL;
    }

    struct macroList *current = macroTable;

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
            return currentMacro; // Found matching macro, return pointer to it
        }

        current = current->nextMacro; // Move to the next node in the macroList
    }

    return NULL; // Macro not found, return NULL
}


int is_Valid_macro_name(const char *name)
{
    /* Check if name is a reserved keyword */
    const char *reservedKeywords[] = {
            "mov", "cmp", "add", "sub", "not", "clr", "lea", "inc",
            "de", "jmp", "bne", "red", "prn", "jsr", "rts", "stop"};
    int numReservedKeywords = sizeof(reservedKeywords) / sizeof(reservedKeywords[0]);

    for (int i = 0; i < numReservedKeywords; i++)
    {
        if (strcmp(name, reservedKeywords[i]) == 0)
        {
            return 0; /* Name is a reserved keyword */
        }
    }

    /* Check if the first character is a letter */
    if (!isalpha(name[0]))
    {
        return 0; /* First character is not a letter */
    }

    /* Check if the name contains only valid characters (letters and digits) */
    for (int j = 0; j < strlen(name); j++)
    {
        if (!isalnum(name[j]))
        {
            return 0; /* Name contains an invalid character */
        }
    }

    /* Name passes all checks */
    return 1;
}

int is_valid_macro_def(const char *line) {
    char *token = NULL;

    /* Allocate memory for mod_line and check if malloc was successful */
    char *mod_line = malloc(strlen(line) + 1);
    if (mod_line == NULL) {
        /* Memory allocation failed; handle the error appropriately */
        return 0;
    }

    /* Copy the line into mod_line before calling strtok */
    strcpy(mod_line, line);

    /* Use strtok to tokenize the line */
    char *first_word = strtok(mod_line, " ");

    if (first_word != NULL && strcmp(first_word, "mcro") != 0) {
        /* The first word is not "mcro", which is required for a valid macro definition */
        free(mod_line);
        return 0;
    }

    /* Get the next token (macro name) */
    token = strtok(NULL, " ");
    if (token == NULL || !is_Valid_macro_name(token)) {
        /* Either the token is NULL or the macro name is invalid */
        free(mod_line);
        return 0;
    }

    /* Check if there are any more tokens (should be no more tokens for a valid macro definition) */
    token = strtok(NULL, " ");
    if (token != NULL) {
        /* There are more tokens, which is not allowed in a valid macro definition */
        free(mod_line);
        return 0;
    }

    /* If all conditions are satisfied, the macro definition is valid */
    free(mod_line);
    return 1;
}




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


void free_content_list(struct content *list) {
    while (list != NULL) {
        struct content *temp = list; /* Store the current node in a temporary variable */
        list = list->next; /* Move to the next node before deallocating the current one */

        /* Free the memory for the current content node */
        free(temp);
    }
}


void write_line_to_file(const char *line, FILE *target_file) {
    fprintf(target_file, "%s\n", line);
}