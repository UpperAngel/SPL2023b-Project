#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "first-pass-headers/data_arr.h"
#include "first-pass-headers/first_pass.h"
#include "first-pass-headers/instruction.h"
#include "first-pass-headers/instruction_helper.h"
#include "first-pass-headers/symbol_table.h"

/* Define a custom assertion-like function for testing. */
void assert_test(int condition, const char *message) {
    if (condition) {
        printf("[PASSED] %s\n", message);
    } else {
        printf("[FAILED] %s\n", message);
    }
}

/* Custom testing function for encode_instruction. */
void test_encode_instruction() {
    int i;
    SymbolTable *table;
    table = init_table(10);
    EncodedInstruction *encoded;
    encoded = (EncodedInstruction *)malloc(sizeof(EncodedInstruction));
    // Test instructions
    const char *instructions[] = {
        "mov 5, HELLO",
        "add r1, 10",
        "sub @r3, symbol1",
        "jmp symbol2",
        "invalid_instruction",
    };

    EncodedInstruction *expacted_resaults;
    expacted_resaults->encoding_type = 2;
    expacted_resaults->opcode = 0;
    expacted_resaults->source_addressing = IMMEDIATE;
    expacted_resaults->target_addressing = DIRECT;

    encode_instruction(encoded, instructions[0], table);
    assert_test(encoded->encoding_type == expacted_resaults->encoding_type, "encode_instruction - encoding_type");
    assert_test(encoded->source_addressing == expacted_resaults->source_addressing, "encode_instruction - source_addresing");
    assert_test(encoded->target_addressing == expacted_resaults->target_addressing, "encode_instruction - target_addresing");
    assert_test(encoded->opcode == expacted_resaults->opcode, "encode_instruction - opcode");
}

/* Custom testing function for is_valid_line_length. */
void test_is_valid_line_length() {
    /* Test case 1: Line length is within the limit. */
    const char *valid_line = "This is a valid line.";
    assert_test(is_valid_line_length(valid_line), "is_valid_line_length - valid line");

    /* Test case 2: Line length exceeds the limit. */
    const char *invalid_line = "This line exceeds the maximum allowed length. It's too long.                                  ";
    assert_test(!is_valid_line_length(invalid_line), "is_valid_line_length - invalid line");
}

/* Custom testing function for is_data. */
void test_is_data() {
    /* Test case 1: Check for a valid data directive. */
    const char *data_directive = ".data";
    assert_test(is_data(data_directive), "is_data - valid data directive");

    /* Test case 2: Check for a valid data directive (different directive from the list). */
    const char *another_data_directive = ".string";
    assert_test(is_data(another_data_directive), "is_data - valid data directive");

    /* Test case 3: Check for an invalid directive. */
    const char *invalid_directive = ".invalid";
    assert_test(!is_data(invalid_directive), "is_data - invalid directive");
}

/* Custom testing function for is_entry. */
void test_is_entry() {
    /* Test case 1: Check for a valid entry directive. */
    const char *entry_directive = ".entry";
    assert_test(is_entry(entry_directive), "is_entry - valid entry directive");

    /* Test case 2: Check for an invalid directive. */
    const char *invalid_directive = ".invalid";
    assert_test(!is_entry(invalid_directive), "is_entry - invalid directive");
}

/* Custom testing function for is_extern. */
void test_is_extern() {
    /* Test case 1: Check for a valid extern directive. */
    const char *extern_directive = ".extern";
    assert_test(is_extern(extern_directive), "is_extern - valid extern directive");

    /* Test case 2: Check for an invalid directive. */
    const char *invalid_directive = ".invalid";
    assert_test(!is_extern(invalid_directive), "is_extern - invalid directive");
}

/* Custom testing function for is_directive. */
void test_is_directive() {
    /* Test case 1: Check for a valid directive. */
    const char *directive = ".data";
    assert_test(is_directive(directive), "is_directive - valid directive");

    /* Test case 2: Check for an invalid directive. */
    const char *non_directive = "NotADirective";
    assert_test(!is_directive(non_directive), "is_directive - invalid directive");
}

/* Custom testing function for the SymbolTable functions. */
void test_symbol_table() {
    SymbolTable *test_table;

    test_table = init_table(10);

    add_symbol(test_table, "s1", 12, CODE);
    add_symbol(test_table, "s2", 1, DATA);
    add_symbol(test_table, "s3", 14, EXTERN);

    print_symbol(get_symbol_by_name(test_table, "s1"));
    print_table(test_table);
    /* Add your test cases for the SymbolTable functions here. */
    /* Remember to test the init_table, add_symbol, get_symbol_by_name, and free_symbol_table functions. */
    /* You may also need to test edge cases and error conditions. */
}

void test_data_arr() {
    int i, j;
    DataArr *data_arr;
    data_arr = init_data_arr(10);

    for (i = 0; i < 5; i++) {
        add_string(data_arr, " hello world");
    }

    for (j = 0; j < 5; j++) {
        add_int(data_arr, 1);
    }
    print_chr_arr(data_arr);
    print_int_arr(data_arr);
}

void test_is_register() {
    char *test_op1 = "@r1";
    char *test_op2 = "test";
    
    int test1 = is_register(test_op1); 
    int test2 = is_register(test_op2); 

    assert_test(test1, "is_register - correct");
    assert_test(!test2, "is_register - invalid");
}

/* Main function to run all the tests. */
int main() {
    printf("Running Tests...\n");
    test_encode_instruction();
    test_is_valid_line_length();
    test_is_data();
    test_is_entry();
    test_is_extern();
    test_is_directive();
    test_is_register();
    printf("\n");
    test_symbol_table();
    test_data_arr();
    return 0;
}
