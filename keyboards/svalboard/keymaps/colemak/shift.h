#pragma once

#include "keymap_support.h"

typedef struct {
    bool phys;
    bool caps;
    bool effective;
} shift_state_t;

void break_caps_word(uint16_t kc);
shift_state_t get_shift_state(void);
