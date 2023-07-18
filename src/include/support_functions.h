/* Maximum length for a line of content */
#define MAXLEN 80
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Struct to represent a line of content */
struct content
{
    char line[MAXLEN];
    struct content *next;
};

typedef struct content node;

/* Struct to represent a macro */
typedef struct
{
    char *name;
    node *data;
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
void add_line_to_mcro(mcro *macro, char *contentSource);

/**
 * Creates a new macro list.
 *
 * @return The newly created macro list.
 */
struct macroList *createMacroList();

/**
 * Creates a new macro with the given name.
 *
 * @param name The name of the macro.
 * @return The newly created macro.
 */
mcro *create_mcro(const char *name);

/**
 * Adds a macro to the macro table.
 *
 * @param macroToAdd The macro to be added to the macro table.
 * @param macroTablePtr Pointer to the macro table.
 */
void add_to_macro_table(mcro *macroToAdd, struct macroList **macroTablePtr);

/**
 * Traverses the macroList and checks whether there is a macro with a specified name.
 *
 * @param macroTable The macroList to be searched.
 * @param name The name of the macro to find.
 * @return Pointer to the matching macro if found, or NULL if not found.
 */
mcro *find_macro_by_name(struct macroList *macroTable, const char *name);

int is_Valid_macro_name(const char *name);
/**
 * Checks if a given macro name adheres to the specified rules.
 *
 * The name must meet the following requirements:
 * - The first character must be a letter.
 * - The name can only contain letters (uppercase or lowercase) and numbers.
 * - The name cannot be one of the reserved keywords: mov, cmp, add, sub, not, clr, lea, inc,
 *   de, jmp, bne, red, prn, jsr, rts, stop.
 *
 * @param name The name to be checked.
 * @return 1 if the name is valid, 0 otherwise.
 */

int is_valid_macro_def(const char *line);
/**
 * checks if the line is a valid definition of a macro
 * @param line the line that is checked.
 * @return 1 if the definition is valid, otherwise 0
 */


void deploy_macro(const mcro *macro, FILE *target_file);
/**
 * expands the macro
*/


void free_macro_list(struct macroList *list);

void free_content_list(struct content *list);