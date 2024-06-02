#include "constants.h"
#include "memory.h"
#include "types.h"
#include "unity.h"
#include <stdlib.h>
#include <string.h>

void setUp(void) {}

void tearDown(void) {}

void test__create_memory__no_input__returns_non_null(void) {
    // Act
    memory_s *memory = create_memory();

    // Assert
    TEST_ASSERT_NOT_NULL(memory);

    // Cleanup
    if (memory) {
        free_memory(memory);
    }
}

void test__create_memory__no_input__allocates_correct_size(void) {
    // Act
    memory_s *memory = create_memory();

    // Assert
    TEST_ASSERT_NOT_NULL(memory);
    TEST_ASSERT_EQUAL(MEMORY_CAPACITY, memory->size);

    // Cleanup
    free_memory(memory);
}

void test__create_memory__no_input__all_memory_initialized_to_zero(void) {
    // Arrange
    byte *junk = (byte *)malloc(MEMORY_CAPACITY);
    memset(junk, 1, MEMORY_CAPACITY);
    free(junk);

    // Act
    memory_s *memory = create_memory();

    // Assert
    TEST_ASSERT_NOT_NULL(memory);

    for (size_t i = 0; i < memory->size; i++) {
        TEST_ASSERT_EQUAL_UINT8(BYTE_MIN_VALUE, memory->memory[i]);
    }

    // Cleanup
    free_memory(memory);
}

void test__create_memory__no_input__address_initialized_to_zero(void) {
    // Arrange
    memory_s *memory = create_memory();
    TEST_ASSERT_NOT_NULL(memory);
    memory->address_msb_register = 1;
    memory->address_lsb_register = 1;
    free_memory(memory);

    // Act
    memory = create_memory();

    // Assert
    TEST_ASSERT_NOT_NULL(memory);

    TEST_ASSERT_EQUAL_UINT8(BYTE_MIN_VALUE, memory->address_msb_register);
    TEST_ASSERT_EQUAL_UINT8(BYTE_MIN_VALUE, memory->address_lsb_register);

    // Cleanup
    free_memory(memory);
}

void test__memory_get_current_register__selecting_pre_set_value__returns_expected_value(
    void) {
    // Arrange
    memory_s *memory = create_memory();
    TEST_ASSERT_NOT_NULL(memory);

    byte expected = 1;

    memory->memory[memory->size - 1] = expected;

    memory->address_msb_register = 255;
    memory->address_lsb_register = 255;

    // Act
    byte actual = memory_get_current_register(memory);

    // Assert
    TEST_ASSERT_EQUAL_UINT8(expected, actual);

    // Cleanup
    free_memory(memory);
}

void test__memory_set_current_register__setting_expected_value__memory_contains_expected_value(
    void) {
    // Arrange
    memory_s *memory = create_memory();
    TEST_ASSERT_NOT_NULL(memory);

    byte expected = 1;

    memory->address_msb_register = 255;
    memory->address_lsb_register = 255;

    // Act
    memory_set_current_register(memory, expected);
    byte actual = memory->memory[memory->size - 1];

    // Assert
    TEST_ASSERT_EQUAL_UINT8(expected, actual);

    // Cleanup
    free_memory(memory);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test__create_memory__no_input__returns_non_null);
    RUN_TEST(test__create_memory__no_input__allocates_correct_size);
    RUN_TEST(test__create_memory__no_input__all_memory_initialized_to_zero);
    RUN_TEST(test__create_memory__no_input__address_initialized_to_zero);
    RUN_TEST(
        test__memory_get_current_register__selecting_pre_set_value__returns_expected_value);
    RUN_TEST(
        test__memory_set_current_register__setting_expected_value__memory_contains_expected_value);
    return UNITY_END();
}
