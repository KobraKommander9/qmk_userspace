#pragma once

#include "layers.h"
#include "keys.h"

typedef struct {
    bool is_press_action;
    uint8_t step;
} tap;

uint8_t dance_step(tap_dance_state_t *state);

extern tap_dance_action_t tap_dance_actions[4];

enum {
    SINGLE_TAP = 1,
    SINGLE_HOLD,
    DOUBLE_TAP,
    DOUBLE_HOLD,
    DOUBLE_SINGLE_TAP,
    MORE_TAPS
};

enum tap_dance_codes {
    DANCE_V,
    DANCE_SCREENSHOT,
    DANCE_MEDIA,
};

// Dance keys
#define TD0 TD(DANCE_V)
#define TD1 TD(DANCE_SCREENSHOT)
#define TD2 TD(DANCE_MEDIA)

void on_dance_0(tap_dance_state_t *state, void *user_data);
void dance_0_finished(tap_dance_state_t *state, void *user_data);
void dance_0_reset(tap_dance_state_t *state, void *user_data);

void on_dance_1(tap_dance_state_t *state, void *user_data);
void dance_1_finished(tap_dance_state_t *state, void *user_data);
void dance_1_reset(tap_dance_state_t *state, void *user_data);

void on_dance_2(tap_dance_state_t *state, void *user_data);
void dance_2_finished(tap_dance_state_t *state, void *user_data);
void dance_2_reset(tap_dance_state_t *state, void *user_data);
