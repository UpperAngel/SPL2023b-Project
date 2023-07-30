#include "first-pass-headers/data_arr.h"

#define UNUSED_CHR 0xFF
#define UNUSED_INT -4096

struct dataArr
{
    int element_num;
    int size;
    char *chr_arr; /* need to be cast into char *[] */
    int *int_arr;
};

DataArr *init_data_arr(const int init_size)
{
    int i;
    DataArr *ret_arr;

    ret_arr = (DataArr *)malloc(sizeof(DataArr) + sizeof(int) * init_size + sizeof(char *) * init_size);
    if (ret_arr == NULL)
    {
        return NULL;
    }

    ret_arr->size = init_size;
    ret_arr->element_num = 0;

    ret_arr->chr_arr = (char *)malloc(sizeof(char) * init_size);
    if (ret_arr->chr_arr == NULL)
    {
        return NULL;
    }

    ret_arr->int_arr = malloc(sizeof(int) * init_size);

    for (i = 0; i < init_size; i++)
    {
        ret_arr->chr_arr[i] = UNUSED_CHR;
        ret_arr->int_arr[i] = UNUSED_INT;
    }

    return ret_arr;
}

void resize_arrays(DataArr* data_arr) {
    int new_size = data_arr->size * 2;
    data_arr->int_arr = (int*)realloc(data_arr->int_arr, new_size * sizeof(int));
    data_arr->chr_arr = (char*)realloc(data_arr->chr_arr, new_size * sizeof(char));

    // Initialize new elements to UNUSED_INT and UNUSED_CHR
    for (int i = data_arr->size; i < new_size; i++) {
        data_arr->int_arr[i] = UNUSED_INT;
        data_arr->chr_arr[i] = UNUSED_CHR;
    }

    data_arr->size = new_size;
}

int add_string(DataArr* data_arr, const char* str) {
    int str_length = strlen(str);

    // Check if there's enough space in the arrays
    if (data_arr->element_num + str_length >= data_arr->size) {
        resize_arrays(data_arr);
    }

    int index = data_arr->element_num;

    // Copy the characters from str to chr_arr
    for (int j = 0; j < str_length; j++) {
        data_arr->chr_arr[index + j] = str[j];
    }

    // Add the null terminator at the end of the string
    data_arr->chr_arr[index + str_length] = '\0';

    data_arr->element_num += str_length + 1; // +1 to account for the null terminator

    return 1;
}

int add_int(DataArr* data_arr, const int num) {
    // Check if there's enough space in the arrays
    if (data_arr->element_num >= data_arr->size) {
        resize_arrays(data_arr);
    }

    int index = data_arr->element_num;
    data_arr->int_arr[index] = num;
    data_arr->chr_arr[index] = UNUSED_CHR;

    data_arr->element_num++;

    return 1;
}

void free_data_arr(DataArr* data_arr) {
    free(data_arr->int_arr);
    free(data_arr->chr_arr);
    free(data_arr);
}



void print_chr_arr(DataArr *data_arr) {
    int i;
    for  (i = 0; i < data_arr->element_num; i++)
    {
        if (data_arr->chr_arr[i] != UNUSED_CHR)
        {
            if (data_arr->chr_arr[i] == '\0')
            {
                printf("\t\t");
            }
            printf("%c", data_arr->chr_arr[i]);
        }
        
    }
    printf("\n"); // Add a newline at the end to ensure proper output formatting
    
}


void print_int_arr(DataArr* data_arr) {
    int i;
    for (i = 0; i < data_arr->element_num; i++) {
        if (data_arr->int_arr[i] != UNUSED_INT) {
            printf(" %d,", data_arr->int_arr[i]);
        }
    }
    printf("\n"); // Add a newline at the end to ensure proper output formatting
}
