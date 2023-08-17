#include "src/first-pass/first-pass-headers/symbol_table.h"

int main(int argc, char const *argv[]) {
    SymbolTable *table = init_table(1);
    add_symbol(&table, "s1", 1, DATA);
    add_symbol(&table, "s1", 1, DATA);
    add_symbol(&table, "s3", 3, DATA);
    add_symbol(&table, "s4", 4, DATA);

    print_table(table);
    free(table);
    return 0;
}
