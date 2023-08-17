#ifndef __DATA_ARR_H
#define __DATA_ARR_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct dataArr DataArr;
DataArr *init_data_arr(const int);
int add_int(DataArr *, const int);
int add_string(DataArr *, const char *);

void print_chr_arr(DataArr *);
void print_int_arr(DataArr *);

#endif