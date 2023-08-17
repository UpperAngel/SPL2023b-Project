#include "../first-pass/first-pass-headers/first_pass_headers.h"
#include "../error-handling/errors.h"
/*
gets name of label, index.
lookup name -> go to index -> encode
*/

void binaryToBase64(uint64_t binary, char *base64);

void second_pass(const char *file_name, Symbol *symbol_table, struct InstructionStructure **instruction_array, struct DataStructure *data_array, struct SymbolNameAndIndex *symbol_name_and_index, int *error_found) {
    struct SymbolNameAndIndex *temp = symbol_name_and_index;
    struct InstructiomStructure **printed_inst_arry = instruction_array;
    struct DataStructure *printed_data_arry = data_array;
    Symbol *temp_symbol;
    FILE *ent_file = NULL;
    FILE *ext_file = NULL;
    FILE *ob_file;
    int entry_flag = 0, extern_flag = 0;
    int inst_array_index, data_array_index;
    int ic = 0, dc = 0;
    int i, j;

    // 2DO - create arr_size func (or get it from first pass)
    for (inst_array_index = 0; inst_array_index < 1024; inst_array_index++) {
        if (instruction_array[inst_array_index] != 0) {
            ic++;
        }
    }

    for (data_array_index = 0; data_array_index < 1024; data_array_index++) {
        if (data_array[data_array_index].value != 0) {
            dc++;
        }
    }

    // move up 
    if (*error_found == 1) {
        return;
    }

    // move up
    struct SymbolNameAndIndex *curr_missing = symbol_name_and_index;
    
    
    while (curr_missing != NULL) {
        Symbol *symbol = get_symbol_by_name(symbol_table, curr_missing->name);
        if (symbol != NULL) {
            write_encoding_type(instruction_array[curr_missing->IC_index], (symbol->type == EXTERN) ? 0x3 : 0x1);
            encode_bits_2_to_11(symbol->val, instruction_array[curr_missing->IC_index]);
        } else {
            *error_found = 1;  /* Found an error */
            return;
        }
        curr_missing = curr_missing->next;
    }

    temp_symbol = symbol_table;
    temp = symbol_name_and_index;
    while (temp_symbol != NULL && temp != NULL)
    {
        if (find_symbol(temp_symbol, getNextName(temp, temp->name) == NULL))
        {
            *error_found = 1;
        }
        

        temp = temp->next;
        temp_symbol = temp_symbol->next;
    }
    
    if (*error_found == 1) {
        return;
    }

    // File handling
    // 2DO - move to another func
    while (symbol_table != NULL) {
        if (symbol_table->type = ENTRY) {
            entry_flag = 1;
            break;
        }
        symbol_table = symbol_table->next;
    }

    while (symbol_table != NULL) {
        if (symbol_table->type = EXTERN) {
            extern_flag = 1;
            break;
        }
        symbol_table = symbol_table->next;
    }

    if (entry_flag) {
       ent_file = create_file(file_name, ".ent");
    }
    if (extern_flag) {
        ext_file = create_file(file_name, ".ext");
    }
    ob_file = create_file(file_name, ".ob");

    while (symbol_table != NULL) {
        if (symbol_table->type == ENTRY) {
            fprintf(ent_file, "%s: %d\n", symbol_table->name, symbol_table->val);
        }

        temp = symbol_name_and_index;
        while (temp != NULL) {
            if (symbol_table->type == EXTERN) {
                fprintf(ext_file, "%s: %d\n", symbol_table->name, getNodeByName(temp, symbol_table->name) != NULL ? temp->IC_index : -1);
            }
            temp = temp->next;
        }

        symbol_table = symbol_table->next;
    }

    fprintf(ob_file, "%d %d", ic, dc);
    for (i = 0; i < ic; i++) {
        char *base64out[2] = "\0";
        binaryToBase64(instruction_array[i], base64out);
        fprintf(ob_file, "%s", base64out);
    }
    for (j = 0; j < dc; i++) {
        char *base64out[2] = "\0";
        binaryToBase64(data_array[j].value, base64out);
        fprintf(ob_file, "%s", base64out);
    }
    fclose(ext_file);
    fclose(ob_file);
    fclose(ent_file);
    // Free resources in the main function
}

void binaryToBase64(uint64_t binary, char *base64) {
    const char base64_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    int base64_length = 0;

    while (binary > 0) {
        int remainder = binary & 0x3F;  /* Get the lowest 6 bits */
        base64[base64_length++] = base64_chars[remainder];
        binary >>= 6;  /* Shift right by 6 bits */
    }

    if (base64_length == 0) {
        base64[base64_length++] = 'A';
    }

    base64[base64_length] = '\0';
}
