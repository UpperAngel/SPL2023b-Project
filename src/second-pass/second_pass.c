#include "second_pass.h"

/* This function performs the second pass of the assembler, generating the output files
 * based on the provided symbol information and instruction/data arrays.
 *
 * Parameters:
 *   - file_name: The name of the output files to be generated.
 *   - curr_symbol: A linked list of symbols.
 *   - instruction_array: An array of InstructionStructure containing encoded instructions.
 *   - data_array: An array of DataStructure containing data values.
 *   - symbol_name_and_index: A linked list of SymbolNameAndIndex representing symbols and their indices.
 *   - ic: The Instruction Counter value.
 *   - dc: The Data Counter value.
 *   - error_found: A pointer to an integer flag indicating if errors were found.
 */
void second_pass(const char *file_name, Symbol *curr_symbol, struct InstructionStructure instruction_array[], struct DataStructure data_array[], struct SymbolNameAndIndex *symbol_name_and_index, int ic, int dc, int *error_found) {
    struct SymbolNameAndIndex *temp = symbol_name_and_index;
    struct SymbolNameAndIndex *curr_missing = symbol_name_and_index;
    Symbol *curr_symbol_copy = curr_symbol;

    if (*error_found == 1) {
        return; /* Abort if errors were previously found */
    }

    /* Resolve missing symbols */
    while (curr_missing != NULL) {
        Symbol *symbol = find_symbol(curr_symbol, curr_missing->name);
        if (symbol != NULL) {
            encoding_A_R_E(&instruction_array[symbol_name_and_index->IC_index], (symbol->category == EXTERN) ? BIT2MASK : -1);
            if (instruction_array[symbol_name_and_index->IC_index].encoding_type == -1) {
                *error_found = 1; /* Set error flag if encoding failed */
            }
            encode_bits_2_to_11(symbol->val, instruction_array);
        } else {
            *error_found = 1; /* Found an error */
            return;
        }
        curr_missing = curr_missing->next;
    }

    /* Check for symbols not used */
    curr_symbol_copy = curr_symbol;
    temp = symbol_name_and_index;
    while (curr_symbol_copy != NULL && temp != NULL) {
        if (find_symbol(curr_symbol_copy, temp->name) == NULL) {
            *error_found = 1; /* Set error flag if unused symbol found */
        }
        temp = temp->next;
        curr_symbol_copy = curr_symbol_copy->next;
    }

    if (*error_found == 1) {
        return; /* Abort if errors were found */
    }

    /* Create output files */
    create_files(file_name, curr_symbol_copy, symbol_name_and_index, instruction_array, data_array, ic, dc);
    free(curr_symbol_copy);
    free(temp);
}

/* This function creates output files containing assembly information.
 *
 * Parameters:
 *   - file_name: The base name for the output files.
 *   - curr_symbol: A linked list of symbols.
 *   - symbol_name_and_index: A linked list of SymbolNameAndIndex representing symbols and their indices.
 *   - instruction_array: An array of InstructionStructure containing encoded instructions.
 *   - data_array: An array of DataStructure containing data values.
 *   - IC: The Instruction Counter value.
 *   - DC: The Data Counter value.
 */
void create_files(const char *file_name, Symbol *curr_symbol, struct SymbolNameAndIndex *symbol_name_and_index, struct InstructionStructure instruction_array[], struct DataStructure data_array[], int IC, int DC) {
    int entry_flag = 1, extern_flag = 1;
    int i = 0, j = 0;

    Symbol *print_extern = curr_symbol;
    Symbol *print_entry = curr_symbol;
    Symbol *find_extern = curr_symbol;
    Symbol *find_entry = curr_symbol;

    struct SymbolNameAndIndex *temp;

    FILE *ob_file = NULL;
    FILE *ent_file = NULL;
    FILE *ext_file = NULL;

    /* Check for entry and extern flags */
    while (find_entry != NULL) {
        if (find_entry->category == ENTRY) {
            entry_flag = 1;
            break;
        }
        find_entry = find_entry->next;
    }
    while (find_extern != NULL) {
        if (find_extern->category == EXTERN) {
            extern_flag = 1;
            break;
        }
        find_extern = find_extern->next;
    }

    /* Create output files */
    if (entry_flag) {
        ent_file = create_file(file_name, ".ent");
    }
    if (extern_flag) {
        ext_file = create_file(file_name, ".ext");
    }
    ob_file = create_file(file_name, ".ob");

    /* Print entry and extern information */
    while (print_entry != NULL) {
        if (print_entry->category == ENTRY) {
            fprintf(ent_file, "%s: %d\n", print_entry->name, print_entry->val);
        }
        temp = symbol_name_and_index;
        while (temp != NULL) {
            if (print_extern->category == EXTERN) {
                fprintf(ext_file, "%s: %d\n", print_extern->name, get_node_by_name(temp, print_extern->name) != NULL ? temp->IC_index : -1);
            }
            temp = temp->next;
        }
        curr_symbol = curr_symbol->next;
    }

    /* Print instruction and data information */
    fprintf(ob_file, "%d %d\n", IC, DC);
    for (i = 0; i < IC; i++) {
        char *base64out = NULL;
        binaryToBase64(pack_instruction(instruction_array[i]), base64out);
        fprintf(ob_file, "%s\n", base64out);
    }
    for (j = 0; j < DC; j++) {
        char *base64out = NULL;
        binaryToBase64(data_array[j].value, base64out);
        fprintf(ob_file, "%s\n", base64out);
    }

    /* Close files and free memory */
    if (ob_file != NULL) {
        fclose(ob_file);
    }
    if (ent_file != NULL) {
        fclose(ent_file);
    }
    if (ext_file != NULL) {
        fclose(ext_file);
    }
    free(print_entry);
    free(print_extern);
    free(find_entry);
    free(find_extern);
}

/* This function converts a binary value to a Base64 string.
 *
 * Parameters:
 *   - binary: The binary value to be converted.
 *   - base64: A pointer to the resulting Base64 string.
 */
void binaryToBase64(uint64_t binary, char *base64) {
    const char base64_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    int base64_length = 0;

    /* Convert binary value to Base64 */
    while (binary > 0) {
        int remainder = binary & 0x3F; /* Get the lowest 6 bits */
        base64[base64_length++] = base64_chars[remainder];
        binary >>= 6; /* Shift right by 6 bits */
    }

    /* Handle special case when binary is 0 */
    if (base64_length == 0) {
        base64[base64_length++] = '\0';
    }
}