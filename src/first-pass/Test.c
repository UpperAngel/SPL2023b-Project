#include "/home/david/CLionProjects/My-project/Project main branch/src/Error handling/handle_error.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#define LEN 200
#define ARRAY_SIZE 1024

/* in c90 standard minimum width of int is 16 bits so we can use the first 12 bits of it*/
struct DataStructure {
    int value : 12;
};
struct DataStructure* create_data_array(int size) {
    // Allocate memory for the array of DataStructure
    struct DataStructure* dataArray = (struct DataStructure*)malloc(size * sizeof(struct DataStructure));

    // Check if memory allocation is successful
    if (dataArray == NULL) {
        // Handle memory allocation failure if needed
        return NULL;
    }

    // Initialize the array elements (optional)
    for (int i = 0; i < size; i++) {
        dataArray[i].value = 0;
    }

    return dataArray;
}
void write_integer(struct DataStructure* data, int number) {
    // Ensure that the integer value fits within the 12-bit range
    if (number >= -2048 && number <= 2047) {
        data->value = number & 0xFFF; // Mask the value to keep only the 12 least significant bits (LSBs)
    } else {
        // Handle the case when the number is out of range for a 12-bit signed integer.
        // You can choose to set an error code or handle it differently as per your requirement.
    }
}
void write_character_to_dataStructure(struct DataStructure* data, char c) {
    // Convert the char to int and store it in the struct
    data->value = (int)c;
}

int is_string_directive(char *word) {
    if (strcmp(word,".string") == 0)
        return 1;
    return 0;
}

int is_data_directive(char *word){
    if (strcmp(word,".data") == 0)
        return 1;
    return 0;
}





/* By the project definition vlaid characters are visible ASCI chars which are chars between 32 and 126 including */
int is_valid_char(char c) {
    return (c >= 32 && c <= 126);
}

int is_valid_string(char *string, int length) {
    int i;
    for (i = 0; i < length; i++) {
        char current_char = string[i];
        if (current_char == '\\') {
            // Found backslash, skip the next character (it's valid)
            i++;
        } else if (current_char == '"') {
            // Found single quote without a preceding backslash, it's an error
            return 0;
        } else if (!is_valid_char(current_char)) {
            return 0;
        }
    }
    return 1;
}

int valid_string_directive(char words_array[LEN][LEN], int line_number, int *error_found, int symbol_definition) {
    char *string;
    int string_length = 0;
    int index = 1;

    if (symbol_definition)
        index++;

    string = words_array[index];
    string_length = strlen(string);

    /* Check if the word is an empty string */
    if (string_length == 0) {
        *error_found = 1;
         handle_error(EmptyStringDirective, line_number);
        return 0;
    }

    /* Check if there are any additional words after the string directive */
    if (strcmp(words_array[index + 1], "") != 0) {
        *error_found = 1;
        handle_error(ExcessCharactersInDataLine, line_number);
        return 0;
    }

        /* Check if the word has at least three characters (starting '"', content, ending '"') */
    else if (string[0] != '"' || (string_length == 1 && string[0] == '"')) {
        *error_found = 1;
        handle_error(InvalidStartCharInString, line_number);
        return 0;
    } else if (string_length > 1 && string[string_length - 1] != '"') {
        *error_found = 1;
        handle_error(InvalidEndCharInString, line_number);
        return 0;
    }

        // Check the characters between the '"' characters (excluding the starting and ending '"')
    else {
        if (!is_valid_string(&string[1], string_length - 2)) {
            *error_found = 1;
            handle_error(InvalidCharInString, line_number);
            return 0;
        }
    }

    /* passed all checks */
    return 1;
}



void test_valid_string_directive() {
    int error_found;
    char words_array[LEN][LEN];

    printf("Test 1: Empty string directive\n");
    strcpy(words_array[0], "STRING");
    strcpy(words_array[1], "");
    error_found = 0;
    int result = valid_string_directive(words_array, 1, &error_found, 0);
    printf("Expected: Error in line 1: Empty string directive\n");
    printf("Actual: %s\n\n", result ? "Pass" : "Fail");

    printf("Test 2: Excessive characters in data line\n");
    strcpy(words_array[0], "STRING");
    strcpy(words_array[1], "This is a string directive");
    strcpy(words_array[2], "with extra characters");
    error_found = 0;
    result = valid_string_directive(words_array, 1, &error_found, 0);
    printf("Expected: Error in line 1: Excess characters at the end of macro definition\n");
    printf("Actual: %s\n\n", result ? "Pass" : "Fail");

    // Add more test cases here...
}

int main() {
    test_valid_string_directive();
    return 0;
}



