#include "constants.h"
#include "emulator.h"
#include "memory.h"
#include "unity.h"

void setUp(void) {}

void tearDown(void) {}

void test__create_emulator__default_memory__returns_non_null(void) {
    // Arrange
    memory_s *memory = create_memory();

    // Assert
    TEST_ASSERT_NOT_NULL(memory);

    // Act
    emulator_s *emulator = create_emulator(memory);

    // Assert
    TEST_ASSERT_NOT_NULL(emulator);

    // Cleanup
    if (emulator) {
        free_emulator(emulator);
    }
    if (memory) {
        free_memory(memory);
    }
}

void test__create_emulator__default_memory__all_values_initalized_to_default_values(
    void) {
    // Arrange
    memory_s *memory = create_memory();

    // Assert
    TEST_ASSERT_NOT_NULL(memory);

    // Act
    emulator_s *emulator = create_emulator(memory);

    // Assert
    TEST_ASSERT_NOT_NULL(emulator);
    TEST_ASSERT_NOT_NULL(emulator->control_unit);

    for (size_t i = 0; i < emulator->control_unit->memory->size; i++) {
        TEST_ASSERT_EQUAL_UINT8(BYTE_MIN_VALUE,
                                emulator->control_unit->memory->memory[i]);
    }
    TEST_ASSERT_EQUAL_UINT8(BYTE_MIN_VALUE,
                            emulator->control_unit->accumulator);
    TEST_ASSERT_EQUAL_UINT8(BYTE_MIN_VALUE,
                            emulator->control_unit->pc_msb_register);
    TEST_ASSERT_EQUAL_UINT8(BYTE_MIN_VALUE,
                            emulator->control_unit->pc_lsb_register);
    TEST_ASSERT_EQUAL_UINT8(BYTE_MIN_VALUE,
                            emulator->control_unit->temp_register);
    TEST_ASSERT_EQUAL_UINT8(BYTE_MIN_VALUE,
                            emulator->control_unit->mar_msb_register);
    TEST_ASSERT_EQUAL_UINT8(BYTE_MIN_VALUE,
                            emulator->control_unit->mar_lsb_register);
    TEST_ASSERT_EQUAL_UINT8(BYTE_INCREMENT,
                            emulator->control_unit->increment_register);

    TEST_ASSERT_NOT_NULL(emulator->control_unit->control_unit_state);

    TEST_ASSERT_EQUAL_UINT8(HALT,
                            emulator->control_unit->control_unit_state->opcode);
    TEST_ASSERT_EQUAL_UINT8(
        STEP_FETCH, emulator->control_unit->control_unit_state->cycle_step);
    TEST_ASSERT_EQUAL_UINT8(
        LOW, emulator->control_unit->control_unit_state->is_halt);

    // Cleanup
    if (emulator) {
        free_emulator(emulator);
    }
    if (memory) {
        free_memory(memory);
    }
}

void test__step__call_once__nothing_happens(void) {
    // Arrange
    memory_s *memory = create_memory();

    // Assert
    TEST_ASSERT_NOT_NULL(memory);

    // Arrange
    emulator_s *emulator = create_emulator(memory);

    // Assert
    TEST_ASSERT_NOT_NULL(emulator);
    TEST_ASSERT_NOT_NULL(emulator->control_unit);

    TEST_ASSERT_NOT_NULL(emulator->step);

    // Act
    emulator->step(emulator);

    // Cleanup
    if (emulator) {
        free_emulator(emulator);
    }
    if (memory) {
        free_memory(memory);
    }
}

void test__cycle__call_once__nothing_happens(void) {
    // Arrange
    memory_s *memory = create_memory();

    // Assert
    TEST_ASSERT_NOT_NULL(memory);

    // Arrange
    emulator_s *emulator = create_emulator(memory);

    // Assert
    TEST_ASSERT_NOT_NULL(emulator);
    TEST_ASSERT_NOT_NULL(emulator->control_unit);

    TEST_ASSERT_NOT_NULL(emulator->cycle);

    // Act
    emulator->cycle(emulator);

    // Cleanup
    if (emulator) {
        free_emulator(emulator);
    }
    if (memory) {
        free_memory(memory);
    }
}

void test__cycle_until_halt__call_once__nothing_happens(void) {
    // Arrange
    memory_s *memory = create_memory();

    // Assert
    TEST_ASSERT_NOT_NULL(memory);

    // Arrange
    emulator_s *emulator = create_emulator(memory);

    // Assert
    TEST_ASSERT_NOT_NULL(emulator);
    TEST_ASSERT_NOT_NULL(emulator->control_unit);

    TEST_ASSERT_NOT_NULL(emulator->cycle_until_halt);

    // Act
    emulator->cycle_until_halt(emulator);

    // Cleanup
    if (emulator) {
        free_emulator(emulator);
    }
    if (memory) {
        free_memory(memory);
    }
}

void test__dump__default_memory__returns_expected_values(void) {
    // Arrange
    memory_s *memory = create_memory();

    // Assert
    TEST_ASSERT_NOT_NULL(memory);

    // Arrange
    emulator_s *emulator = create_emulator(memory);

    // Assert
    TEST_ASSERT_NOT_NULL(emulator);
    TEST_ASSERT_NOT_NULL(emulator->control_unit);

    TEST_ASSERT_NOT_NULL(emulator->dump);

    // Act
    state_s *state = emulator->dump(emulator);

    // Assert
    TEST_ASSERT_NOT_NULL(state);

    TEST_ASSERT_NOT_NULL(state->memory);
    TEST_ASSERT_EQUAL(MEMORY_CAPACITY, state->memory_size);

    for (size_t i = 0; i < state->memory_size; i++) {
        TEST_ASSERT_EQUAL_UINT8(BYTE_MIN_VALUE, state->memory[i]);
    }

    TEST_ASSERT_EQUAL_UINT8(BYTE_MIN_VALUE, state->accumulator);
    TEST_ASSERT_EQUAL_UINT8(BYTE_MIN_VALUE, state->pc_msb_register);
    TEST_ASSERT_EQUAL_UINT8(BYTE_MIN_VALUE, state->pc_lsb_register);
    TEST_ASSERT_EQUAL_UINT8(BYTE_MIN_VALUE, state->temp_register);
    TEST_ASSERT_EQUAL_UINT8(BYTE_MIN_VALUE, state->mar_msb_register);
    TEST_ASSERT_EQUAL_UINT8(BYTE_MIN_VALUE, state->mar_lsb_register);

    TEST_ASSERT_EQUAL_UINT8(HALT, state->opcode);
    TEST_ASSERT_EQUAL_UINT8(STEP_FETCH, state->cycle_step);
    TEST_ASSERT_EQUAL_UINT8(LOW, state->is_halt);

    // Cleanup
    if (state) {
        free(state);
    }
    if (emulator) {
        free_emulator(emulator);
    }
    if (memory) {
        free_memory(memory);
    }
}

void test__step__stepping_full_instruction_cycle__dump_validates_cycle_steps(
    void) {
    // Arrange
    memory_s *memory = create_memory();

    // Assert
    TEST_ASSERT_NOT_NULL(memory);

    // Arrange
    emulator_s *emulator = create_emulator(memory);
    state_s *state;

    // Assert
    TEST_ASSERT_NOT_NULL(emulator);
    TEST_ASSERT_NOT_NULL(emulator->control_unit);

    TEST_ASSERT_NOT_NULL(emulator->dump);

    // Act and assert
    state = emulator->dump(emulator);
    TEST_ASSERT_NOT_NULL(state);
    TEST_ASSERT_EQUAL_UINT8(STEP_FETCH, state->cycle_step);
    free(state);

    emulator->step(emulator);
    state = emulator->dump(emulator);
    TEST_ASSERT_NOT_NULL(state);
    TEST_ASSERT_EQUAL_UINT8(STEP_DECODE, state->cycle_step);
    free(state);

    emulator->step(emulator);
    state = emulator->dump(emulator);
    TEST_ASSERT_NOT_NULL(state);
    TEST_ASSERT_EQUAL_UINT8(STEP_EVALUATE_ADDRESS_MSB, state->cycle_step);
    free(state);

    emulator->step(emulator);
    state = emulator->dump(emulator);
    TEST_ASSERT_NOT_NULL(state);
    TEST_ASSERT_EQUAL_UINT8(STEP_FETCH_OPERAND_MSB, state->cycle_step);
    free(state);

    emulator->step(emulator);
    state = emulator->dump(emulator);
    TEST_ASSERT_NOT_NULL(state);
    TEST_ASSERT_EQUAL_UINT8(STEP_EVALUATE_ADDRESS_LSB, state->cycle_step);
    free(state);

    emulator->step(emulator);
    state = emulator->dump(emulator);
    TEST_ASSERT_NOT_NULL(state);
    TEST_ASSERT_EQUAL_UINT8(STEP_FETCH_OPERAND_LSB, state->cycle_step);
    free(state);

    emulator->step(emulator);
    state = emulator->dump(emulator);
    TEST_ASSERT_NOT_NULL(state);
    TEST_ASSERT_EQUAL_UINT8(STEP_EXECUTE, state->cycle_step);
    free(state);

    emulator->step(emulator);
    state = emulator->dump(emulator);
    TEST_ASSERT_NOT_NULL(state);
    TEST_ASSERT_EQUAL_UINT8(STEP_STORE_RESULT, state->cycle_step);
    free(state);

    emulator->step(emulator);
    state = emulator->dump(emulator);
    TEST_ASSERT_NOT_NULL(state);
    TEST_ASSERT_EQUAL_UINT8(STEP_INCREMENT_PC, state->cycle_step);
    free(state);

    emulator->step(emulator);
    state = emulator->dump(emulator);

    // Assert
    TEST_ASSERT_NOT_NULL(state->memory);
    TEST_ASSERT_EQUAL(MEMORY_CAPACITY, state->memory_size);

    for (size_t i = 0; i < state->memory_size; i++) {
        TEST_ASSERT_EQUAL_UINT8(BYTE_MIN_VALUE, state->memory[i]);
    }

    TEST_ASSERT_EQUAL_UINT8(BYTE_MIN_VALUE, state->accumulator);
    TEST_ASSERT_EQUAL_UINT8(BYTE_MIN_VALUE, state->pc_msb_register);
    TEST_ASSERT_EQUAL_UINT8(BYTE_MIN_VALUE, state->pc_lsb_register);
    TEST_ASSERT_EQUAL_UINT8(BYTE_MIN_VALUE, state->temp_register);
    TEST_ASSERT_EQUAL_UINT8(BYTE_MIN_VALUE, state->mar_msb_register);
    TEST_ASSERT_EQUAL_UINT8(BYTE_MIN_VALUE, state->mar_lsb_register);

    TEST_ASSERT_EQUAL_UINT8(HALT, state->opcode);
    TEST_ASSERT_EQUAL_UINT8(STEP_FETCH, state->cycle_step);
    TEST_ASSERT_EQUAL_UINT8(HIGH, state->is_halt);

    // Cleanup
    if (state) {
        free(state);
    }
    if (emulator) {
        free_emulator(emulator);
    }
    if (memory) {
        free_memory(memory);
    }
}

void test__cycle__cycle_once__dump_contains_expected_values(void) {
    // Arrange
    memory_s *memory = create_memory();

    // Assert
    TEST_ASSERT_NOT_NULL(memory);

    // Arrange
    emulator_s *emulator = create_emulator(memory);
    state_s *state;

    // Assert
    TEST_ASSERT_NOT_NULL(emulator);
    TEST_ASSERT_NOT_NULL(emulator->control_unit);

    TEST_ASSERT_NOT_NULL(emulator->dump);

    // Act and assert
    emulator->cycle(emulator);
    state = emulator->dump(emulator);

    // Assert
    TEST_ASSERT_NOT_NULL(state->memory);
    TEST_ASSERT_EQUAL(MEMORY_CAPACITY, state->memory_size);

    for (size_t i = 0; i < state->memory_size; i++) {
        TEST_ASSERT_EQUAL_UINT8(BYTE_MIN_VALUE, state->memory[i]);
    }

    TEST_ASSERT_EQUAL_UINT8(BYTE_MIN_VALUE, state->accumulator);
    TEST_ASSERT_EQUAL_UINT8(BYTE_MIN_VALUE, state->pc_msb_register);
    TEST_ASSERT_EQUAL_UINT8(BYTE_MIN_VALUE, state->pc_lsb_register);
    TEST_ASSERT_EQUAL_UINT8(BYTE_MIN_VALUE, state->temp_register);
    TEST_ASSERT_EQUAL_UINT8(BYTE_MIN_VALUE, state->mar_msb_register);
    TEST_ASSERT_EQUAL_UINT8(BYTE_MIN_VALUE, state->mar_lsb_register);

    TEST_ASSERT_EQUAL_UINT8(HALT, state->opcode);
    TEST_ASSERT_EQUAL_UINT8(STEP_FETCH, state->cycle_step);
    TEST_ASSERT_EQUAL_UINT8(HIGH, state->is_halt);

    // Cleanup
    if (state) {
        free(state);
    }
    if (emulator) {
        free_emulator(emulator);
    }
    if (memory) {
        free_memory(memory);
    }
}

void test__cycle_until_halt__cycle_until_halt__dump_contains_expected_values(
    void) {
    // Arrange
    memory_s *memory = create_memory();

    // Assert
    TEST_ASSERT_NOT_NULL(memory);

    // Arrange
    emulator_s *emulator = create_emulator(memory);
    state_s *state;

    // Assert
    TEST_ASSERT_NOT_NULL(emulator);
    TEST_ASSERT_NOT_NULL(emulator->control_unit);

    TEST_ASSERT_NOT_NULL(emulator->dump);

    // Act and assert
    emulator->cycle_until_halt(emulator);
    state = emulator->dump(emulator);

    // Assert
    TEST_ASSERT_NOT_NULL(state->memory);
    TEST_ASSERT_EQUAL(MEMORY_CAPACITY, state->memory_size);

    for (size_t i = 0; i < state->memory_size; i++) {
        TEST_ASSERT_EQUAL_UINT8(BYTE_MIN_VALUE, state->memory[i]);
    }

    TEST_ASSERT_EQUAL_UINT8(BYTE_MIN_VALUE, state->accumulator);
    TEST_ASSERT_EQUAL_UINT8(BYTE_MIN_VALUE, state->pc_msb_register);
    TEST_ASSERT_EQUAL_UINT8(BYTE_MIN_VALUE, state->pc_lsb_register);
    TEST_ASSERT_EQUAL_UINT8(BYTE_MIN_VALUE, state->temp_register);
    TEST_ASSERT_EQUAL_UINT8(BYTE_MIN_VALUE, state->mar_msb_register);
    TEST_ASSERT_EQUAL_UINT8(BYTE_MIN_VALUE, state->mar_lsb_register);

    TEST_ASSERT_EQUAL_UINT8(HALT, state->opcode);
    TEST_ASSERT_EQUAL_UINT8(STEP_FETCH, state->cycle_step);
    TEST_ASSERT_EQUAL_UINT8(HIGH, state->is_halt);

    // Cleanup
    if (state) {
        free(state);
    }
    if (emulator) {
        free_emulator(emulator);
    }
    if (memory) {
        free_memory(memory);
    }
}

void test__cycle__cycle_load_instruction__accumulator_contains_expected_value(
    void) {
    // Arrange
    byte expected = 1;

    memory_s *memory = create_memory();

    // Assert
    TEST_ASSERT_NOT_NULL(memory);

    // Arrange
    memory->memory[0] = LOAD;
    memory->memory[1] = 255;
    memory->memory[2] = 255;
    memory->memory[memory->size - 1] = expected;

    emulator_s *emulator = create_emulator(memory);
    state_s *state;

    // Assert
    TEST_ASSERT_NOT_NULL(emulator);

    // Act
    emulator->cycle(emulator);
    state = emulator->dump(emulator);

    // Assert
    TEST_ASSERT_NOT_NULL(state);

    TEST_ASSERT_EQUAL_UINT8(expected, state->accumulator);

    // Cleanup
    if (state) {
        free(state);
    }
    if (emulator) {
        free_emulator(emulator);
    }
    if (memory) {
        free_memory(memory);
    }
}

void test__cycle__cycle_load_and_store_instructions__accumulator_contains_expected_value(
    void) {
    // Arrange
    byte expected = 1;

    memory_s *memory = create_memory();

    // Assert
    TEST_ASSERT_NOT_NULL(memory);

    // Arrange
    memory->memory[0] = LOAD;
    memory->memory[1] = 0;
    memory->memory[2] = 6;
    memory->memory[3] = STORE;
    memory->memory[4] = 255;
    memory->memory[5] = 255;
    memory->memory[6] = expected;

    emulator_s *emulator = create_emulator(memory);
    state_s *state;

    // Assert
    TEST_ASSERT_NOT_NULL(emulator);

    // Act
    emulator->cycle(emulator);
    emulator->cycle(emulator);
    state = emulator->dump(emulator);

    // Assert
    TEST_ASSERT_NOT_NULL(state);

    TEST_ASSERT_EQUAL_UINT8(expected, state->accumulator);

    // Cleanup
    if (state) {
        free(state);
    }
    if (emulator) {
        free_emulator(emulator);
    }
    if (memory) {
        free_memory(memory);
    }
}

void test__cycle_until_halt__simple_example_program__halts_and_last_memory_location_contains_expected_value(
    void) {
    // Arrange
    byte expected = 50;

    memory_s *memory = create_memory();

    // Assert
    TEST_ASSERT_NOT_NULL(memory);

    // Arrange
    memory->memory[0] = LOAD;
    // 00 00 LOAD
    memory->memory[1] = 0;
    // 00 01 00
    memory->memory[2] = 10;
    // 00 02 0A
    memory->memory[3] = ADD;
    // 00 03 ADD
    memory->memory[4] = 0;
    // 00 04 00
    memory->memory[5] = 11;
    // 00 05 0B
    memory->memory[6] = STORE;
    // 00 06 STORE
    memory->memory[7] = 255;
    // 00 07 FF
    memory->memory[8] = 255;
    // 00 08 FF
    memory->memory[9] = HALT;
    // 00 09 HALT
    memory->memory[10] = 20;
    // 00 0A 14; 20
    memory->memory[11] = 30;
    // 00 0B 1E; 30

    emulator_s *emulator = create_emulator(memory);
    state_s *state;

    // Assert
    TEST_ASSERT_NOT_NULL(emulator);

    // Act
    emulator->cycle_until_halt(emulator);
    state = emulator->dump(emulator);

    // Assert
    TEST_ASSERT_NOT_NULL(state);

    TEST_ASSERT_EQUAL_UINT8(expected, state->memory[state->memory_size - 1]);
    TEST_ASSERT_EQUAL_UINT8(HIGH, state->is_halt);

    // Cleanup
    if (state) {
        free(state);
    }
    if (emulator) {
        free_emulator(emulator);
    }
    if (memory) {
        free_memory(memory);
    }
}

void test__cycle_until_halt__advanced_example_program_that_tests_all_opcodes__halts_and_last_memory_location_contains_expected_value(
    void) {
    // Arrange
    byte expected = 95;

    memory_s *memory = create_memory();

    // Assert
    TEST_ASSERT_NOT_NULL(memory);

    // Arrange
    memory->memory[0] = JMP;
    // 00 00 JMP
    memory->memory[1] = 254;
    // 00 01 FE
    memory->memory[2] = 0;
    // 00 02 00

    memory->memory[256] = 55;
    // 01 00 37; 55
    memory->memory[257] = 20;
    // 01 01 14; 20
    memory->memory[258] = 2;
    // 01 02 02;  2
    memory->memory[259] = 1;
    // 01 03 01;  1

    memory->memory[65024] = LOAD;
    // FE 00 LOAD
    memory->memory[65025] = 1;
    // FE 01 01
    memory->memory[65026] = 0;
    // FE 02 00
    memory->memory[65027] = ADD;
    // FE 03 ADD
    memory->memory[65028] = 1;
    // FE 04 01
    memory->memory[65029] = 2;
    // FE 05 02
    memory->memory[65030] = STORE;
    // FE 06 STORE
    memory->memory[65031] = 1;
    // FE 07 01
    memory->memory[65032] = 0;
    // FE 08 00
    memory->memory[65033] = LOAD;
    // FE 09 LOAD
    memory->memory[65034] = 1;
    // FE 0A 01
    memory->memory[65035] = 1;
    // FE 0B 01
    memory->memory[65036] = SUB;
    // FE 0C SUB
    memory->memory[65037] = 1;
    // FE 0D 01
    memory->memory[65038] = 3;
    // FE 0E 03
    memory->memory[65039] = STORE;
    // FE 0F STORE
    memory->memory[65040] = 1;
    // FE 10 01
    memory->memory[65041] = 1;
    // FE 11 01
    memory->memory[65042] = LOAD;
    // FE 12 LOAD
    memory->memory[65043] = 1;
    // FE 13 01
    memory->memory[65044] = 1;
    // FE 14 01
    memory->memory[65045] = JZ;
    // FE 15 JZ
    memory->memory[65046] = 255;
    // FE 16 FF
    memory->memory[65047] = 0;
    // FE 17 00
    memory->memory[65048] = JMP;
    // FE 18 JMP
    memory->memory[65049] = 254;
    // FE 19 FE
    memory->memory[65050] = 0;
    // FE 1A 00

    memory->memory[65280] = LOAD;
    // FF 00 LOAD
    memory->memory[65281] = 1;
    // FF 01 01
    memory->memory[65282] = 0;
    // FF 02 00
    memory->memory[65283] = STORE;
    // FF 03 STORE
    memory->memory[65284] = 255;
    // FF 04 FF
    memory->memory[65285] = 255;
    // FF 05 FF
    memory->memory[65286] = HALT;
    // FF 06 HALT

    emulator_s *emulator = create_emulator(memory);
    state_s *state;

    // Assert
    TEST_ASSERT_NOT_NULL(emulator);

    // Act
    emulator->cycle_until_halt(emulator);
    state = emulator->dump(emulator);

    // Assert
    TEST_ASSERT_NOT_NULL(state);

    TEST_ASSERT_EQUAL_UINT8(expected, state->memory[state->memory_size - 1]);
    TEST_ASSERT_EQUAL_UINT8(HIGH, state->is_halt);

    // Cleanup
    if (state) {
        free(state);
    }
    if (emulator) {
        free_emulator(emulator);
    }
    if (memory) {
        free_memory(memory);
    }
}

void test__cycle__jz_works_as_expected_after_add__accumulator_and_pc_contains_expected_values(
    void) {
    // Arrange
    byte expected_accumulator = 1;
    byte expected_msb_register = 0;
    byte expected_lsb_register = 9;

    memory_s *memory = create_memory();

    // Assert
    TEST_ASSERT_NOT_NULL(memory);

    // Arrange
    memory->memory[0] = LOAD;
    memory->memory[1] = 1;
    memory->memory[2] = 0;
    memory->memory[3] = ADD;
    memory->memory[4] = 1;
    memory->memory[5] = 1;
    memory->memory[6] = JZ;
    memory->memory[7] = 255;
    memory->memory[8] = 255;

    memory->memory[256] = 0;
    memory->memory[257] = 1;

    emulator_s *emulator = create_emulator(memory);
    state_s *state;

    // Assert
    TEST_ASSERT_NOT_NULL(emulator);

    // Act
    emulator->cycle(emulator);
    emulator->cycle(emulator);
    emulator->cycle(emulator);
    state = emulator->dump(emulator);

    // Assert
    TEST_ASSERT_NOT_NULL(state);

    TEST_ASSERT_EQUAL_UINT8(expected_accumulator, state->accumulator);
    TEST_ASSERT_EQUAL_UINT8(expected_msb_register, state->pc_msb_register);
    TEST_ASSERT_EQUAL_UINT8(expected_lsb_register, state->pc_lsb_register);

    // Cleanup
    if (state) {
        free(state);
    }
    if (emulator) {
        free_emulator(emulator);
    }
    if (memory) {
        free_memory(memory);
    }
}

void test__cycle__jz_works_as_expected_after_sub__accumulator_and_pc_contains_expected_values(
    void) {
    // Arrange
    byte expected_accumulator = 255;
    byte expected_msb_register = 0;
    byte expected_lsb_register = 9;

    memory_s *memory = create_memory();

    // Assert
    TEST_ASSERT_NOT_NULL(memory);

    // Arrange
    memory->memory[0] = LOAD;
    memory->memory[1] = 1;
    memory->memory[2] = 0;
    memory->memory[3] = SUB;
    memory->memory[4] = 1;
    memory->memory[5] = 1;
    memory->memory[6] = JZ;
    memory->memory[7] = 255;
    memory->memory[8] = 255;

    memory->memory[256] = 0;
    memory->memory[257] = 1;

    emulator_s *emulator = create_emulator(memory);
    state_s *state;

    // Assert
    TEST_ASSERT_NOT_NULL(emulator);

    // Act
    emulator->cycle(emulator);
    emulator->cycle(emulator);
    emulator->cycle(emulator);
    state = emulator->dump(emulator);

    // Assert
    TEST_ASSERT_NOT_NULL(state);

    TEST_ASSERT_EQUAL_UINT8(expected_accumulator, state->accumulator);
    TEST_ASSERT_EQUAL_UINT8(expected_msb_register, state->pc_msb_register);
    TEST_ASSERT_EQUAL_UINT8(expected_lsb_register, state->pc_lsb_register);

    // Cleanup
    if (state) {
        free(state);
    }
    if (emulator) {
        free_emulator(emulator);
    }
    if (memory) {
        free_memory(memory);
    }
}

void test__cycle__chaining_load_add_store_sub_jz__accumulator_and_pc_and_stored_value_contains_expected_values(
    void) {
    // Arrange
    byte expected_accumulator = 0;
    byte expected_msb_register = 255;
    byte expected_lsb_register = 255;
    byte expected_stored_value = 1;

    memory_s *memory = create_memory();

    // Assert
    TEST_ASSERT_NOT_NULL(memory);

    // Arrange
    memory->memory[0] = LOAD;
    memory->memory[1] = 1;
    memory->memory[2] = 0;
    memory->memory[3] = ADD;
    memory->memory[4] = 1;
    memory->memory[5] = 1;
    memory->memory[6] = STORE;
    memory->memory[7] = 255;
    memory->memory[8] = 0;
    memory->memory[9] = SUB;
    memory->memory[10] = 1;
    memory->memory[11] = 1;
    memory->memory[12] = JZ;
    memory->memory[13] = 255;
    memory->memory[14] = 255;

    memory->memory[256] = 0;
    memory->memory[257] = 1;

    emulator_s *emulator = create_emulator(memory);
    state_s *state;

    // Assert
    TEST_ASSERT_NOT_NULL(emulator);

    // Act
    int i;
    for (i = 0; i < 5; i++) {
        emulator->cycle(emulator);
    }
    state = emulator->dump(emulator);

    // Assert
    TEST_ASSERT_NOT_NULL(state);

    TEST_ASSERT_EQUAL_UINT8(expected_accumulator, state->accumulator);
    TEST_ASSERT_EQUAL_UINT8(expected_msb_register, state->pc_msb_register);
    TEST_ASSERT_EQUAL_UINT8(expected_lsb_register, state->pc_lsb_register);
    TEST_ASSERT_EQUAL_UINT8(expected_stored_value, state->memory[65280]);

    // Cleanup
    if (state) {
        free(state);
    }
    if (emulator) {
        free_emulator(emulator);
    }
    if (memory) {
        free_memory(memory);
    }
}

void test__cycle__chaining_load_add_store_sub_sub_jz__accumulator_and_pc_and_stored_value_contains_expected_values(
    void) {
    // Arrange
    byte expected_accumulator = 255;
    byte expected_msb_register = 0;
    byte expected_lsb_register = 18;
    byte expected_stored_value = 1;

    memory_s *memory = create_memory();

    // Assert
    TEST_ASSERT_NOT_NULL(memory);

    // Arrange
    memory->memory[0] = LOAD;
    memory->memory[1] = 1;
    memory->memory[2] = 0;
    memory->memory[3] = ADD;
    memory->memory[4] = 1;
    memory->memory[5] = 1;
    memory->memory[6] = STORE;
    memory->memory[7] = 255;
    memory->memory[8] = 0;
    memory->memory[9] = SUB;
    memory->memory[10] = 1;
    memory->memory[11] = 1;
    memory->memory[12] = SUB;
    memory->memory[13] = 1;
    memory->memory[14] = 1;
    memory->memory[15] = JZ;
    memory->memory[16] = 255;
    memory->memory[17] = 255;

    memory->memory[256] = 0;
    memory->memory[257] = 1;

    emulator_s *emulator = create_emulator(memory);
    state_s *state;

    // Assert
    TEST_ASSERT_NOT_NULL(emulator);

    // Act
    int i;
    for (i = 0; i < 6; i++) {
        emulator->cycle(emulator);
    }
    state = emulator->dump(emulator);

    // Assert
    TEST_ASSERT_NOT_NULL(state);

    TEST_ASSERT_EQUAL_UINT8(expected_accumulator, state->accumulator);
    TEST_ASSERT_EQUAL_UINT8(expected_msb_register, state->pc_msb_register);
    TEST_ASSERT_EQUAL_UINT8(expected_lsb_register, state->pc_lsb_register);
    TEST_ASSERT_EQUAL_UINT8(expected_stored_value, state->memory[65280]);

    // Cleanup
    if (state) {
        free(state);
    }
    if (emulator) {
        free_emulator(emulator);
    }
    if (memory) {
        free_memory(memory);
    }
}

void test__cycle__chaining_load_add_store_jz__accumulator_and_pc_and_stored_value_contains_expected_values(
    void) {
    // Arrange
    byte expected_accumulator = 2;
    byte expected_msb_register = 0;
    byte expected_lsb_register = 12;
    byte expected_stored_value = 2;

    memory_s *memory = create_memory();

    // Assert
    TEST_ASSERT_NOT_NULL(memory);

    // Arrange
    memory->memory[0] = LOAD;
    memory->memory[1] = 1;
    memory->memory[2] = 0;
    memory->memory[3] = ADD;
    memory->memory[4] = 1;
    memory->memory[5] = 0;
    memory->memory[6] = STORE;
    memory->memory[7] = 255;
    memory->memory[8] = 0;
    memory->memory[9] = JZ;
    memory->memory[10] = 255;
    memory->memory[11] = 255;

    memory->memory[256] = 1;

    emulator_s *emulator = create_emulator(memory);
    state_s *state;

    // Assert
    TEST_ASSERT_NOT_NULL(emulator);

    // Act
    emulator->cycle(emulator);
    emulator->cycle(emulator);
    emulator->cycle(emulator);
    emulator->cycle(emulator);
    state = emulator->dump(emulator);

    // Assert
    TEST_ASSERT_NOT_NULL(state);

    TEST_ASSERT_EQUAL_UINT8(expected_accumulator, state->accumulator);
    TEST_ASSERT_EQUAL_UINT8(expected_msb_register, state->pc_msb_register);
    TEST_ASSERT_EQUAL_UINT8(expected_lsb_register, state->pc_lsb_register);
    TEST_ASSERT_EQUAL_UINT8(expected_stored_value, state->memory[65280]);

    // Cleanup
    if (state) {
        free(state);
    }
    if (emulator) {
        free_emulator(emulator);
    }
    if (memory) {
        free_memory(memory);
    }
}

void test__cycle__chaining_load_sub_store_jz__accumulator_and_pc_and_stored_value_contains_expected_values(
    void) {
    // Arrange
    byte expected_accumulator = 0;
    byte expected_msb_register = 255;
    byte expected_lsb_register = 255;
    byte expected_stored_value = 0;

    memory_s *memory = create_memory();

    // Assert
    TEST_ASSERT_NOT_NULL(memory);

    // Arrange
    memory->memory[0] = LOAD;
    memory->memory[1] = 1;
    memory->memory[2] = 0;
    memory->memory[3] = SUB;
    memory->memory[4] = 1;
    memory->memory[5] = 0;
    memory->memory[6] = STORE;
    memory->memory[7] = 255;
    memory->memory[8] = 0;
    memory->memory[9] = JZ;
    memory->memory[10] = 255;
    memory->memory[11] = 255;

    memory->memory[256] = 1;

    emulator_s *emulator = create_emulator(memory);
    state_s *state;

    // Assert
    TEST_ASSERT_NOT_NULL(emulator);

    // Act
    emulator->cycle(emulator);
    emulator->cycle(emulator);
    emulator->cycle(emulator);
    emulator->cycle(emulator);
    state = emulator->dump(emulator);

    // Assert
    TEST_ASSERT_NOT_NULL(state);

    TEST_ASSERT_EQUAL_UINT8(expected_accumulator, state->accumulator);
    TEST_ASSERT_EQUAL_UINT8(expected_msb_register, state->pc_msb_register);
    TEST_ASSERT_EQUAL_UINT8(expected_lsb_register, state->pc_lsb_register);
    TEST_ASSERT_EQUAL_UINT8(expected_stored_value, state->memory[65280]);

    // Cleanup
    if (state) {
        free(state);
    }
    if (emulator) {
        free_emulator(emulator);
    }
    if (memory) {
        free_memory(memory);
    }
}

void test__cycle__chaining_load_jmp_add_store_jz__accumulator_and_pc_and_stored_value_contains_expected_values(
    void) {
    // Arrange
    byte expected_accumulator = 2;
    byte expected_msb_register = 0;
    byte expected_lsb_register = 15;
    byte expected_stored_value = 2;

    memory_s *memory = create_memory();

    // Assert
    TEST_ASSERT_NOT_NULL(memory);

    // Arrange
    memory->memory[0] = LOAD;
    memory->memory[1] = 1;
    memory->memory[2] = 0;
    memory->memory[3] = JMP;
    memory->memory[4] = 0;
    memory->memory[5] = 6;
    memory->memory[6] = ADD;
    memory->memory[7] = 1;
    memory->memory[8] = 0;
    memory->memory[9] = STORE;
    memory->memory[10] = 255;
    memory->memory[11] = 0;
    memory->memory[12] = JZ;
    memory->memory[13] = 255;
    memory->memory[14] = 255;

    memory->memory[256] = 1;

    emulator_s *emulator = create_emulator(memory);
    state_s *state;

    // Assert
    TEST_ASSERT_NOT_NULL(emulator);

    // Act
    int i;
    for (i = 0; i < 5; i++) {
        emulator->cycle(emulator);
    }
    state = emulator->dump(emulator);

    // Assert
    TEST_ASSERT_NOT_NULL(state);

    TEST_ASSERT_EQUAL_UINT8(expected_accumulator, state->accumulator);
    TEST_ASSERT_EQUAL_UINT8(expected_msb_register, state->pc_msb_register);
    TEST_ASSERT_EQUAL_UINT8(expected_lsb_register, state->pc_lsb_register);
    TEST_ASSERT_EQUAL_UINT8(expected_stored_value, state->memory[65280]);

    // Cleanup
    if (state) {
        free(state);
    }
    if (emulator) {
        free_emulator(emulator);
    }
    if (memory) {
        free_memory(memory);
    }
}

void test__cycle__chaining_load_jmp_sub_store_jz__accumulator_and_pc_and_stored_value_contains_expected_values(
    void) {
    // Arrange
    byte expected_accumulator = 0;
    byte expected_msb_register = 255;
    byte expected_lsb_register = 255;
    byte expected_stored_value = 0;

    memory_s *memory = create_memory();

    // Assert
    TEST_ASSERT_NOT_NULL(memory);

    // Arrange
    memory->memory[0] = LOAD;
    memory->memory[1] = 1;
    memory->memory[2] = 0;
    memory->memory[3] = JMP;
    memory->memory[4] = 0;
    memory->memory[5] = 6;
    memory->memory[6] = SUB;
    memory->memory[7] = 1;
    memory->memory[8] = 0;
    memory->memory[9] = STORE;
    memory->memory[10] = 255;
    memory->memory[11] = 0;
    memory->memory[12] = JZ;
    memory->memory[13] = 255;
    memory->memory[14] = 255;

    memory->memory[256] = 1;

    emulator_s *emulator = create_emulator(memory);
    state_s *state;

    // Assert
    TEST_ASSERT_NOT_NULL(emulator);

    // Act
    int i;
    for (i = 0; i < 5; i++) {
        emulator->cycle(emulator);
    }
    state = emulator->dump(emulator);

    // Assert
    TEST_ASSERT_NOT_NULL(state);

    TEST_ASSERT_EQUAL_UINT8(expected_accumulator, state->accumulator);
    TEST_ASSERT_EQUAL_UINT8(expected_msb_register, state->pc_msb_register);
    TEST_ASSERT_EQUAL_UINT8(expected_lsb_register, state->pc_lsb_register);
    TEST_ASSERT_EQUAL_UINT8(expected_stored_value, state->memory[65280]);

    // Cleanup
    if (state) {
        free(state);
    }
    if (emulator) {
        free_emulator(emulator);
    }
    if (memory) {
        free_memory(memory);
    }
}

void test__cycle__chaining_load_add_jmp_store_jz__accumulator_and_pc_and_stored_value_contains_expected_values(
    void) {
    // Arrange
    byte expected_accumulator = 2;
    byte expected_msb_register = 0;
    byte expected_lsb_register = 15;
    byte expected_stored_value = 2;

    memory_s *memory = create_memory();

    // Assert
    TEST_ASSERT_NOT_NULL(memory);

    // Arrange
    memory->memory[0] = LOAD;
    memory->memory[1] = 1;
    memory->memory[2] = 0;
    memory->memory[3] = ADD;
    memory->memory[4] = 1;
    memory->memory[5] = 0;
    memory->memory[6] = JMP;
    memory->memory[7] = 0;
    memory->memory[8] = 9;
    memory->memory[9] = STORE;
    memory->memory[10] = 255;
    memory->memory[11] = 0;
    memory->memory[12] = JZ;
    memory->memory[13] = 255;
    memory->memory[14] = 255;

    memory->memory[256] = 1;

    emulator_s *emulator = create_emulator(memory);
    state_s *state;

    // Assert
    TEST_ASSERT_NOT_NULL(emulator);

    // Act
    int i;
    for (i = 0; i < 5; i++) {
        emulator->cycle(emulator);
    }
    state = emulator->dump(emulator);

    // Assert
    TEST_ASSERT_NOT_NULL(state);

    TEST_ASSERT_EQUAL_UINT8(expected_accumulator, state->accumulator);
    TEST_ASSERT_EQUAL_UINT8(expected_msb_register, state->pc_msb_register);
    TEST_ASSERT_EQUAL_UINT8(expected_lsb_register, state->pc_lsb_register);
    TEST_ASSERT_EQUAL_UINT8(expected_stored_value, state->memory[65280]);

    // Cleanup
    if (state) {
        free(state);
    }
    if (emulator) {
        free_emulator(emulator);
    }
    if (memory) {
        free_memory(memory);
    }
}

void test__cycle__chaining_load_sub_jmp_store_jz__accumulator_and_pc_and_stored_value_contains_expected_values(
    void) {
    // Arrange
    byte expected_accumulator = 0;
    byte expected_msb_register = 255;
    byte expected_lsb_register = 255;
    byte expected_stored_value = 0;

    memory_s *memory = create_memory();

    // Assert
    TEST_ASSERT_NOT_NULL(memory);

    // Arrange
    memory->memory[0] = LOAD;
    memory->memory[1] = 1;
    memory->memory[2] = 0;
    memory->memory[3] = SUB;
    memory->memory[4] = 1;
    memory->memory[5] = 0;
    memory->memory[6] = JMP;
    memory->memory[7] = 0;
    memory->memory[8] = 9;
    memory->memory[9] = STORE;
    memory->memory[10] = 255;
    memory->memory[11] = 0;
    memory->memory[12] = JZ;
    memory->memory[13] = 255;
    memory->memory[14] = 255;

    memory->memory[256] = 1;

    emulator_s *emulator = create_emulator(memory);
    state_s *state;

    // Assert
    TEST_ASSERT_NOT_NULL(emulator);

    // Act
    int i;
    for (i = 0; i < 5; i++) {
        emulator->cycle(emulator);
    }
    state = emulator->dump(emulator);

    // Assert
    TEST_ASSERT_NOT_NULL(state);

    TEST_ASSERT_EQUAL_UINT8(expected_accumulator, state->accumulator);
    TEST_ASSERT_EQUAL_UINT8(expected_msb_register, state->pc_msb_register);
    TEST_ASSERT_EQUAL_UINT8(expected_lsb_register, state->pc_lsb_register);
    TEST_ASSERT_EQUAL_UINT8(expected_stored_value, state->memory[65280]);

    // Cleanup
    if (state) {
        free(state);
    }
    if (emulator) {
        free_emulator(emulator);
    }
    if (memory) {
        free_memory(memory);
    }
}

void test__cycle__chaining_load_add_store_jmp_jz__accumulator_and_pc_and_stored_value_contains_expected_values(
    void) {
    // Arrange
    byte expected_accumulator = 2;
    byte expected_msb_register = 0;
    byte expected_lsb_register = 15;
    byte expected_stored_value = 2;

    memory_s *memory = create_memory();

    // Assert
    TEST_ASSERT_NOT_NULL(memory);

    // Arrange
    memory->memory[0] = LOAD;
    memory->memory[1] = 1;
    memory->memory[2] = 0;
    memory->memory[3] = ADD;
    memory->memory[4] = 1;
    memory->memory[5] = 0;
    memory->memory[6] = STORE;
    memory->memory[7] = 255;
    memory->memory[8] = 0;
    memory->memory[9] = JMP;
    memory->memory[10] = 0;
    memory->memory[11] = 12;
    memory->memory[12] = JZ;
    memory->memory[13] = 255;
    memory->memory[14] = 255;

    memory->memory[256] = 1;

    emulator_s *emulator = create_emulator(memory);
    state_s *state;

    // Assert
    TEST_ASSERT_NOT_NULL(emulator);

    // Act
    int i;
    for (i = 0; i < 5; i++) {
        emulator->cycle(emulator);
    }
    state = emulator->dump(emulator);

    // Assert
    TEST_ASSERT_NOT_NULL(state);

    TEST_ASSERT_EQUAL_UINT8(expected_accumulator, state->accumulator);
    TEST_ASSERT_EQUAL_UINT8(expected_msb_register, state->pc_msb_register);
    TEST_ASSERT_EQUAL_UINT8(expected_lsb_register, state->pc_lsb_register);
    TEST_ASSERT_EQUAL_UINT8(expected_stored_value, state->memory[65280]);

    // Cleanup
    if (state) {
        free(state);
    }
    if (emulator) {
        free_emulator(emulator);
    }
    if (memory) {
        free_memory(memory);
    }
}

void test__cycle__chaining_load_sub_store_jmp_jz__accumulator_and_pc_and_stored_value_contains_expected_values(
    void) {
    // Arrange
    byte expected_accumulator = 0;
    byte expected_msb_register = 255;
    byte expected_lsb_register = 255;
    byte expected_stored_value = 0;

    memory_s *memory = create_memory();

    // Assert
    TEST_ASSERT_NOT_NULL(memory);

    // Arrange
    memory->memory[0] = LOAD;
    memory->memory[1] = 1;
    memory->memory[2] = 0;
    memory->memory[3] = SUB;
    memory->memory[4] = 1;
    memory->memory[5] = 0;
    memory->memory[6] = STORE;
    memory->memory[7] = 255;
    memory->memory[8] = 0;
    memory->memory[9] = JMP;
    memory->memory[10] = 0;
    memory->memory[11] = 12;
    memory->memory[12] = JZ;
    memory->memory[13] = 255;
    memory->memory[14] = 255;

    memory->memory[256] = 1;

    emulator_s *emulator = create_emulator(memory);
    state_s *state;

    // Assert
    TEST_ASSERT_NOT_NULL(emulator);

    // Act
    int i;
    for (i = 0; i < 5; i++) {
        emulator->cycle(emulator);
    }
    state = emulator->dump(emulator);

    // Assert
    TEST_ASSERT_NOT_NULL(state);

    TEST_ASSERT_EQUAL_UINT8(expected_accumulator, state->accumulator);
    TEST_ASSERT_EQUAL_UINT8(expected_msb_register, state->pc_msb_register);
    TEST_ASSERT_EQUAL_UINT8(expected_lsb_register, state->pc_lsb_register);
    TEST_ASSERT_EQUAL_UINT8(expected_stored_value, state->memory[65280]);

    // Cleanup
    if (state) {
        free(state);
    }
    if (emulator) {
        free_emulator(emulator);
    }
    if (memory) {
        free_memory(memory);
    }
}

void test__step__call_once__returns_status_ok(void) {
    // Arrange
    memory_s *memory = create_memory();
    status status;

    // Assert
    TEST_ASSERT_NOT_NULL(memory);

    // Arrange
    emulator_s *emulator = create_emulator(memory);

    // Assert
    TEST_ASSERT_NOT_NULL(emulator);
    TEST_ASSERT_NOT_NULL(emulator->control_unit);

    TEST_ASSERT_NOT_NULL(emulator->step);

    // Act
    status = emulator->step(emulator);

    // Assert
    TEST_ASSERT_EQUAL_INT(STATUS_OK, status);

    // Cleanup
    if (emulator) {
        free_emulator(emulator);
    }
    if (memory) {
        free_memory(memory);
    }
}

void test__step__null_pointer__returns_status_error(void) {
    // Arrange
    memory_s *memory = create_memory();
    status status;

    // Assert
    TEST_ASSERT_NOT_NULL(memory);

    // Arrange
    emulator_s *emulator = create_emulator(memory);

    // Assert
    TEST_ASSERT_NOT_NULL(emulator);
    TEST_ASSERT_NOT_NULL(emulator->control_unit);

    TEST_ASSERT_NOT_NULL(emulator->step);

    // Act
    status = emulator->step(NULL);

    // Assert
    TEST_ASSERT_EQUAL_INT(STATUS_ERROR, status);

    // Cleanup
    if (emulator) {
        free_emulator(emulator);
    }
    if (memory) {
        free_memory(memory);
    }
}

void test__cycle__call_once__returns_status_ok(void) {
    // Arrange
    memory_s *memory = create_memory();
    status status;

    // Assert
    TEST_ASSERT_NOT_NULL(memory);

    // Arrange
    emulator_s *emulator = create_emulator(memory);

    // Assert
    TEST_ASSERT_NOT_NULL(emulator);
    TEST_ASSERT_NOT_NULL(emulator->control_unit);

    TEST_ASSERT_NOT_NULL(emulator->cycle);

    // Act
    status = emulator->cycle(emulator);

    // Assert
    TEST_ASSERT_EQUAL_INT(STATUS_OK, status);

    // Cleanup
    if (emulator) {
        free_emulator(emulator);
    }
    if (memory) {
        free_memory(memory);
    }
}

void test__cycle__null_pointer__returns_status_error(void) {
    // Arrange
    memory_s *memory = create_memory();
    status status;

    // Assert
    TEST_ASSERT_NOT_NULL(memory);

    // Arrange
    emulator_s *emulator = create_emulator(memory);

    // Assert
    TEST_ASSERT_NOT_NULL(emulator);
    TEST_ASSERT_NOT_NULL(emulator->control_unit);

    TEST_ASSERT_NOT_NULL(emulator->cycle);

    // Act
    status = emulator->cycle(NULL);

    // Assert
    TEST_ASSERT_EQUAL_INT(STATUS_ERROR, status);

    // Cleanup
    if (emulator) {
        free_emulator(emulator);
    }
    if (memory) {
        free_memory(memory);
    }
}

void test__cycle_until_halt__call_once__returns_status_ok(void) {
    // Arrange
    memory_s *memory = create_memory();
    status status;

    // Assert
    TEST_ASSERT_NOT_NULL(memory);

    // Arrange
    emulator_s *emulator = create_emulator(memory);

    // Assert
    TEST_ASSERT_NOT_NULL(emulator);
    TEST_ASSERT_NOT_NULL(emulator->control_unit);

    TEST_ASSERT_NOT_NULL(emulator->cycle_until_halt);

    // Act
    status = emulator->cycle_until_halt(emulator);

    // Assert
    TEST_ASSERT_EQUAL_INT(STATUS_OK, status);

    // Cleanup
    if (emulator) {
        free_emulator(emulator);
    }
    if (memory) {
        free_memory(memory);
    }
}

void test__cycle_until_halt__null_pointer__returns_status_error(void) {
    // Arrange
    memory_s *memory = create_memory();
    status status;

    // Assert
    TEST_ASSERT_NOT_NULL(memory);

    // Arrange
    emulator_s *emulator = create_emulator(memory);

    // Assert
    TEST_ASSERT_NOT_NULL(emulator);
    TEST_ASSERT_NOT_NULL(emulator->control_unit);

    TEST_ASSERT_NOT_NULL(emulator->cycle_until_halt);

    // Act
    status = emulator->cycle_until_halt(NULL);

    // Assert
    TEST_ASSERT_EQUAL_INT(STATUS_ERROR, status);

    // Cleanup
    if (emulator) {
        free_emulator(emulator);
    }
    if (memory) {
        free_memory(memory);
    }
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test__create_emulator__default_memory__returns_non_null);
    RUN_TEST(
        test__create_emulator__default_memory__all_values_initalized_to_default_values);
    RUN_TEST(test__step__call_once__nothing_happens);
    RUN_TEST(test__cycle__call_once__nothing_happens);
    RUN_TEST(test__cycle_until_halt__call_once__nothing_happens);
    RUN_TEST(test__dump__default_memory__returns_expected_values);
    RUN_TEST(
        test__step__stepping_full_instruction_cycle__dump_validates_cycle_steps);
    RUN_TEST(test__cycle__cycle_once__dump_contains_expected_values);
    RUN_TEST(
        test__cycle_until_halt__cycle_until_halt__dump_contains_expected_values);
    RUN_TEST(
        test__cycle__cycle_load_instruction__accumulator_contains_expected_value);
    RUN_TEST(
        test__cycle__cycle_load_and_store_instructions__accumulator_contains_expected_value);
    RUN_TEST(
        test__cycle_until_halt__simple_example_program__halts_and_last_memory_location_contains_expected_value);
    RUN_TEST(
        test__cycle_until_halt__advanced_example_program_that_tests_all_opcodes__halts_and_last_memory_location_contains_expected_value);
    RUN_TEST(
        test__cycle__jz_works_as_expected_after_add__accumulator_and_pc_contains_expected_values);
    RUN_TEST(
        test__cycle__jz_works_as_expected_after_sub__accumulator_and_pc_contains_expected_values);
    RUN_TEST(
        test__cycle__chaining_load_add_store_sub_jz__accumulator_and_pc_and_stored_value_contains_expected_values);
    RUN_TEST(
        test__cycle__chaining_load_add_store_sub_sub_jz__accumulator_and_pc_and_stored_value_contains_expected_values);
    RUN_TEST(
        test__cycle__chaining_load_add_store_jz__accumulator_and_pc_and_stored_value_contains_expected_values);
    RUN_TEST(
        test__cycle__chaining_load_sub_store_jz__accumulator_and_pc_and_stored_value_contains_expected_values);
    RUN_TEST(
        test__cycle__chaining_load_jmp_add_store_jz__accumulator_and_pc_and_stored_value_contains_expected_values);
    RUN_TEST(
        test__cycle__chaining_load_jmp_sub_store_jz__accumulator_and_pc_and_stored_value_contains_expected_values);
    RUN_TEST(
        test__cycle__chaining_load_add_jmp_store_jz__accumulator_and_pc_and_stored_value_contains_expected_values);
    RUN_TEST(
        test__cycle__chaining_load_sub_jmp_store_jz__accumulator_and_pc_and_stored_value_contains_expected_values);
    RUN_TEST(
        test__cycle__chaining_load_add_store_jmp_jz__accumulator_and_pc_and_stored_value_contains_expected_values);
    RUN_TEST(
        test__cycle__chaining_load_sub_store_jmp_jz__accumulator_and_pc_and_stored_value_contains_expected_values);
    RUN_TEST(test__step__call_once__returns_status_ok);
    RUN_TEST(test__step__null_pointer__returns_status_error);
    RUN_TEST(test__cycle__call_once__returns_status_ok);
    RUN_TEST(test__cycle__null_pointer__returns_status_error);
    RUN_TEST(test__cycle_until_halt__call_once__returns_status_ok);
    RUN_TEST(test__cycle_until_halt__null_pointer__returns_status_error);
    return UNITY_END();
}
