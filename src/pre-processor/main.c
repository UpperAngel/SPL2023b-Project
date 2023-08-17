#include <stdio.h>
#include "../pre-processor header files/pre_processor.h"

void print_file_contents(FILE* file) {
    int ch;
    while ((ch = fgetc(file)) != EOF) {
        putchar(ch);
    }
}

int main(void) {
    FILE *input_file = fopen("input.txt", "r+");
    FILE *output_file = fopen("output.txt", "w");
    int result = 0;
    char buffer[256];

    if (input_file == NULL || output_file == NULL) {
        printf("Error: Failed to open one or both files.\n");
        return 1;
    }

    result = process_file(input_file, output_file);
    if (result) {
        printf("File made successfully\n");
        printf("Output file contents:\n");
        rewind(output_file); // Move the file pointer to the beginning of the output file
        print_file_contents(output_file);
    } else {
        printf("No file was made\n");
    }

    fclose(input_file);
    fclose(output_file);
    return 0;
}
