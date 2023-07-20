#include "pre-processor header files/pre_processor.h"
#include <stdio.h>

int main(void) {
    FILE *input_file = fopen("input.txt", "r+");
    FILE *output_file = fopen("output.txt", "w");

    if (input_file == NULL) {
        printf("Error: Failed to open one or both files.\n");
        return 1;
    }

    int result = process_file(input_file, output_file);
    if (result) {
        printf("File made successfully\n");
    } else {
        printf("No file was made\n");
    }

    fclose(input_file);
    fclose(output_file);
    return 0;
}
