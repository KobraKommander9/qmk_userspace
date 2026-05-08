#pragma once

#include "quantum.h"
#include "keys.h"

typedef struct {
    tap_dance_state_t state;
} td_user_state_t;

typedef enum {
    _TD_CKC_SPC,
    TD_COUNT,
} td_index_t;

extern tap_dance_action_t tap_dance_actions[TD_COUNT];

bool process_custom_dances(uint16_t, keyrecord_t*);
