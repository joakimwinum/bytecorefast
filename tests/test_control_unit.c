#include "constants.h"
#include "control_unit.h"
#include "control_unit_state.h"
#include "memory.h"
#include "unity.h"
#include <stdlib.h>
#include <string.h>

void setUp(void) {}

void tearDown(void) {}

void test__create_control_unit__default_memory__returns_non_null(void) {
    // Arrange
    memory_s *memory = create_memory();

    // Assert
    TEST_ASSERT_NOT_NULL(memory);

    // Act
    control_unit_s *control_unit = create_control_unit(memory);

    // Assert
    TEST_ASSERT_NOT_NULL(control_unit);

    // Cleanup
    if (control_unit) {
        free_control_unit(control_unit);
    }
    if (memory) {
        free_memory(memory);
    }
}

void test__create_control_unit__default_memory__all_values_initalized_to_default_values(
    void) {
    // Arrange
    memory_s *memory = create_memory();

    // Assert
    TEST_ASSERT_NOT_NULL(memory);

    // Act
    control_unit_s *control_unit = create_control_unit(memory);

    // Assert
    TEST_ASSERT_NOT_NULL(control_unit);

    for (size_t i = 0; i < control_unit->memory->size; i++) {
        TEST_ASSERT_EQUAL_UINT8(BYTE_MIN_VALUE,
                                control_unit->memory->memory[i]);
    }
    TEST_ASSERT_EQUAL_UINT8(BYTE_MIN_VALUE, control_unit->accumulator);
    TEST_ASSERT_EQUAL_UINT8(BYTE_MIN_VALUE, control_unit->pc_msb_register);
    TEST_ASSERT_EQUAL_UINT8(BYTE_MIN_VALUE, control_unit->pc_lsb_register);
    TEST_ASSERT_EQUAL_UINT8(BYTE_MIN_VALUE, control_unit->temp_register);
    TEST_ASSERT_EQUAL_UINT8(BYTE_MIN_VALUE, control_unit->mar_msb_register);
    TEST_ASSERT_EQUAL_UINT8(BYTE_MIN_VALUE, control_unit->mar_lsb_register);
    TEST_ASSERT_EQUAL_UINT8(BYTE_INCREMENT, control_unit->increment_register);

    TEST_ASSERT_NOT_NULL(control_unit->control_unit_state);

    TEST_ASSERT_EQUAL_UINT8(HALT, control_unit->control_unit_state->opcode);
    TEST_ASSERT_EQUAL_UINT8(STEP_FETCH,
                            control_unit->control_unit_state->cycle_step);
    TEST_ASSERT_EQUAL_UINT8(LOW, control_unit->control_unit_state->is_halt);

    // Cleanup
    if (control_unit) {
        free_control_unit(control_unit);
    }
    if (memory) {
        free_memory(memory);
    }
}

void test__create_control_unit__set_all_to_new_value__all_has_new_value(void) {
    // Arrange
    memory_s *memory = create_memory();

    // Assert
    TEST_ASSERT_NOT_NULL(memory);

    // Act
    control_unit_s *control_unit = create_control_unit(memory);

    // Assert
    TEST_ASSERT_NOT_NULL(control_unit);
    TEST_ASSERT_NOT_NULL(control_unit->control_unit_state);

    // Arrange
    memset(control_unit->memory->memory, 1, MEMORY_CAPACITY);
    control_unit->accumulator = 1;
    control_unit->pc_msb_register = 1;
    control_unit->pc_lsb_register = 1;
    control_unit->temp_register = 1;
    control_unit->mar_msb_register = 1;
    control_unit->mar_lsb_register = 1;
    control_unit->increment_register = BYTE_MIN_VALUE;

    control_unit->control_unit_state->opcode = 1;
    control_unit->control_unit_state->cycle_step = 1;
    control_unit->control_unit_state->is_halt = HIGH;

    // Assert
    for (size_t i = 0; i < control_unit->memory->size; i++) {
        TEST_ASSERT_EQUAL_UINT8(1, control_unit->memory->memory[i]);
    }
    TEST_ASSERT_EQUAL_UINT8(1, control_unit->accumulator);
    TEST_ASSERT_EQUAL_UINT8(1, control_unit->pc_msb_register);
    TEST_ASSERT_EQUAL_UINT8(1, control_unit->pc_lsb_register);
    TEST_ASSERT_EQUAL_UINT8(1, control_unit->temp_register);
    TEST_ASSERT_EQUAL_UINT8(1, control_unit->mar_msb_register);
    TEST_ASSERT_EQUAL_UINT8(1, control_unit->mar_lsb_register);
    TEST_ASSERT_EQUAL_UINT8(BYTE_MIN_VALUE, control_unit->increment_register);

    TEST_ASSERT_EQUAL_UINT8(1, control_unit->control_unit_state->opcode);
    TEST_ASSERT_EQUAL_UINT8(1, control_unit->control_unit_state->cycle_step);
    TEST_ASSERT_EQUAL_UINT8(HIGH, control_unit->control_unit_state->is_halt);

    // Cleanup
    if (control_unit) {
        free_control_unit(control_unit);
    }
    if (memory) {
        free_memory(memory);
    }
}

void test__create_control_unit__null_memory__returns_null(void) {
    // Arrange
    memory_s *memory = NULL;

    // Assert
    TEST_ASSERT_NULL(memory);

    // Act
    control_unit_s *control_unit = create_control_unit(memory);

    // Assert
    TEST_ASSERT_NULL(control_unit);
}

void test__create_control_unit__smaller_memory__returns_null(void) {
    // Arrange
    memory_s *memory = create_memory();
    memory->size = memory->size - 1;

    // Assert
    TEST_ASSERT_NOT_NULL(memory);

    // Act
    control_unit_s *control_unit = create_control_unit(memory);

    // Assert
    TEST_ASSERT_NULL(control_unit);
}

void test__create_control_unit__bigger_memory__returns_null(void) {
    // Arrange
    memory_s *memory = create_memory();
    memory->size = memory->size + 1;

    // Assert
    TEST_ASSERT_NOT_NULL(memory);

    // Act
    control_unit_s *control_unit = create_control_unit(memory);

    // Assert
    TEST_ASSERT_NULL(control_unit);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test__create_control_unit__default_memory__returns_non_null);
    RUN_TEST(
        test__create_control_unit__default_memory__all_values_initalized_to_default_values);
    RUN_TEST(
        test__create_control_unit__set_all_to_new_value__all_has_new_value);
    RUN_TEST(test__create_control_unit__null_memory__returns_null);
    RUN_TEST(test__create_control_unit__smaller_memory__returns_null);
    RUN_TEST(test__create_control_unit__bigger_memory__returns_null);
    return UNITY_END();
}
