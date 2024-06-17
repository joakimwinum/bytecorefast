#ifndef BYTECOREFAST_EMULATOR_H
#define BYTECOREFAST_EMULATOR_H

#include "control_unit.h"
#include "memory.h"
#include "state.h"

typedef int status;
#define STATUS_OK 0
#define STATUS_ERROR -1
#define STATUS_SIGNAL_DETECTED 1

typedef status (*check_signals_function)();

typedef struct emulator {
    control_unit_s *control_unit;

    // Functions
    status (*step)(struct emulator *self);
    status (*cycle)(struct emulator *self);
    status (*cycle_until_halt)(struct emulator *self,
                               check_signals_function check_signals);
    state_s *(*dump)(struct emulator *self); // remember to call free on state_s
} emulator_s;

emulator_s *create_emulator(memory_s *memory);
void free_emulator(emulator_s *emulator); // This will NOT free memory_s,
                                          // but everything else

#endif // BYTECOREFAST_EMULATOR_H
