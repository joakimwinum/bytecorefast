#ifndef STATE_H
#define STATE_H

#include "types.h"
#include <stdlib.h>

typedef struct {
    byte *memory;
    size_t memory_size;

    byte accumulator;
    byte pc_msb_register;
    byte pc_lsb_register;
    byte temp_register;
    byte mar_msb_register;
    byte mar_lsb_register;

    byte opcode;
    byte cycle_step;
    bit is_halt;
} state_s;

#endif // STATE_H
