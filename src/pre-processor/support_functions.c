#include "../include/support_functions.h"
/* API style documentation of every function is in the "supportFunction.h" file */

void addLineToMcro(mcro* macro, char* contentSource)
{
    /* Pointer to the pointer to the data member */
    struct content** ptr = &(macro->data);

    /* Create a new node */
    struct content* macroNode = (struct content*)malloc(sizeof(struct content));

    /* ********************HERE SHOULD BE ADDED ERROR IF FAILED TO ALLOCATE MEMORY *********/

    /* Copy the content into the new node */
    strcpy(macroNode->line, contentSource);
    macroNode->next = NULL;

    /* If the data member is NULL, update it with the new node */
    if (*ptr == NULL) {
        *ptr = macroNode;
    }
    /* If the data member is not NULL, find the last node and append the new node */
    else {
        while ((*ptr)->next != NULL) {
            ptr = &((*ptr)->next);
        }
        (*ptr)->next = macroNode;
    }
}


void addToMacroTable(mcro* macroToAdd, struct macroList** macroTablePtr)
{
    /* Create a new node */
    struct macroList* newNode = malloc(sizeof(struct macroList));
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
        struct macroList* ptr = *macroTablePtr;

        /* Traverse to the end of the macro table */
        while (ptr->nextMacro != NULL)
        {
            ptr = ptr->nextMacro;
        }

        /* Append the new node to the end of the macro table */
        ptr->nextMacro = newNode;
    }
}



mcro* createMcro(const char* name) {
    mcro* newMcro = malloc(sizeof(mcro));
/********************HERE SHOULD BE ADDED ERROR IF FAILED TO ALLOCATE MEMORY *********/

    if (newMcro != NULL) {
        newMcro->name = strdup(name);  /* Allocate memory and copy the name */
        newMcro->data = NULL;           /* Initialize data member to NULL */
    }

    return newMcro;
}

struct macroList* createMacroList() {
    struct macroList* newList = malloc(sizeof(struct macroList));
/********************HERE SHOULD BE ADDED ERROR IF FAILED TO ALLOCATE MEMORY *********/

    if (newList != NULL) {
        newList->macro = NULL;       /* Initialize macro member to NULL */
        newList->nextMacro = NULL;   /* Initialize nextMacro member to NULL */
    }

    return newList;
}



mcro* findMacroByName(struct macroList* macroTable, const char* name) {
    struct macroList* current = macroTable;

    while (current != NULL) {
        mcro* currentMacro = current->macro;

        /* Compare the current macro's name with the specified name */
        if (strcmp(currentMacro->name, name) == 0) {
            return currentMacro; /* Found matching macro, return pointer to it */
        }

        current = current->nextMacro; /* Move to the next node in the macroList */
    }

    return NULL; /* Macro not found, return NULL */
}


int isValidName(const char* name) {
    /* Check if name is a reserved keyword */
    const char* reservedKeywords[] = {
        "mov", "cmp", "add", "sub", "not", "clr", "lea", "inc",
        "de", "jmp", "bne", "red", "prn", "jsr", "rts", "stop"
    };
    int numReservedKeywords = sizeof(reservedKeywords) / sizeof(reservedKeywords[0]);

    for (int i = 0; i < numReservedKeywords; i++) {
        if (strcmp(name, reservedKeywords[i]) == 0) {
            return 0;  /* Name is a reserved keyword */
        }
    }

    /* Check if the first character is a letter */
    if (!isalpha(name[0])) {
        return 0;  /* First character is not a letter */
    }

    /* Check if the name contains only valid characters (letters and digits) */
    for (int i = 0; i < strlen(name); i++) {
        if (!isalnum(name[i])) {
            return 0;  /* Name contains an invalid character */
        }
    }

    /* Name passes all checks */
    return 1;
}
