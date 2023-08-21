#ifndef first_pass_headers_H
#define first_pass_headers_H

#include "../../globals/globals.h"
#include "encode.h"
#include "../../error-handling/errors.h"
#include "instruction.h"
#include "../../pre-processor/pre-processor-headers/support_functions.h"


/* FUNCTION DECLARATION */




/* get functions */

enum opcode get_opcode(const char* instruction_name_str);

/* FORAMAT LINE FUNCTIONS */
int line_too_long(char *line);
int is_whitespace(char c) ;
int is_empty(const char *line);
int is_comment(const char *line);
int is_comment_or_empty(char *line);
void format_line(char source[LEN], char formatted_line[LEN]) ;
void store_words(char line[], char resultArray[LEN][LEN]) ;
void format_and_store_words(char line[LEN],char words_array[LEN][LEN]);
void update_variables(char **current_symbol_name, int *symbol_definition, int *line_number, int *index);




/* STRING DIRECTIVE */
int valid_string_directive(char *line, int line_number, int *error_found, int words_to_skip);
int handle_string_directive(struct DataStructure *data_array, int DC, char *line, int line_number);
int valid_number_range(char *word) ;
int is_valid_char(char c);
int is_directive(char *word, const char *directive);


/* DATA DIRECTIVE */
int valid_data_directive(char words_array[LEN][LEN], int line_number, int *error_found, int symbol_definition);
void write_character(struct DataStructure* data, char c) ;
int handle_data_directive(struct DataStructure *data_array, int DC, char words_array[LEN][LEN], int starting_index);
int is_data_directive(char *word);
void write_integer(struct DataStructure* data, int number);
int valid_commas_in_directive(char words_array[LEN][LEN], int starting_index, int line_number);
int valid_entry_and_extern_directive(char words_array[LEN][LEN], int *error_found, int line_number, int symbol_definition);
void handle_extern_and_entry_directives(char words_array[LEN][LEN], Symbol **symbol_head, int symbol_definition,int line_number, int *error_found, SymbolType type, SymbolCategory category );


/* SYMBOL DEF */


void handle_symbol(Symbol **head, const char *name, int line_number, int *error_found, SymbolType parameter_type, SymbolCategory parameter_entry_or_extern, int parameter_value);

/* INSTRUCTION HANDLING FUNCTIONS */


/* STRUCT OF SYMBOL HANDLING */


#endif
