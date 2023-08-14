#include <stdlib.h>


/* in c90 standard minimum width of char is 8 bits */
struct InstructionStructure {
    char encoding_type : 2;   // 2-bit field
    char target_addressing : 3; // 3-bit field
    char opcode : 4;         // 4-bit field
    char source_addressing : 3; // 3-bit field
};

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


void write_character(struct DataStructure* data, char c) {
    // Convert the char to int and store it in the struct
    data->value = (int)c;
}
