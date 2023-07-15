/* Maximum length for a line of content */
#define MAXLEN 80

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Struct to represent a line of content */
struct content {
    char line[MAXLEN];
    struct content* next;
};

typedef struct content node;

/* Struct to represent a macro */
typedef struct {
    char* name;
    node* data;
} mcro;

/* Struct to represent a node in the macro table */
struct macroList
{
    mcro *macro;
    struct macroList *nextMacro;
};

/**
 * Adds a line of content to a given macro.
 *
 * @param macro The macro to which the line of content is added.
 * @param contentSource The source of the content to be added.
 */
void addLineToMcro(mcro *macro, char *contentSource);

/**
 * Creates a new macro list.
 *
 * @return The newly created macro list.
 */
struct macroList* createMacroList();

/**
 * Creates a new macro with the given name.
 *
 * @param name The name of the macro.
 * @return The newly created macro.
 */
mcro* createMcro(const char* name);

/**
 * Adds a macro to the macro table.
 *
 * @param macroToAdd The macro to be added to the macro table.
 * @param macroTablePtr Pointer to the macro table.
 */
void addToMacroTable(mcro* macroToAdd, struct macroList** macroTablePtr);


/**
 * Traverses the macroList and checks whether there is a macro with a specified name.
 *
 * @param macroTable The macroList to be searched.
 * @param name The name of the macro to find.
 * @return Pointer to the matching macro if found, or NULL if not found.
 */
mcro* findMacroByName(struct macroList* macroTable, const char* name);
