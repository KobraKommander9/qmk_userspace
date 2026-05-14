#include "tap_dance.h"

typedef enum {
    TD_NONE,
    TD_UNKNOWN,
    TD_SINGLE_TAP,
    TD_SINGLE_HOLD,
    TD_DOUBLE_TAP,
    TD_DOUBLE_HOLD,
    TD_DOUBLE_SINGLE_TAP,
    TD_TRIPLE_TAP,
    TD_TRIPLE_HOLD,
    TD_TRIPLE_SINGLE_TAP,
} td_state_t;

typedef struct {
    td_state_t state;
} td_tap_t;

typedef struct {
    uint16_t tap;
    uint16_t hold;
    uint16_t held;
} tap_dance_tap_hold_t;

#define ACTION_TAP_DANCE_TAP_HOLD(tap, hold)                                \
    {.fn = {NULL, tap_dance_tap_hold_finished, tap_dance_tap_hold_reset},   \
     .user_data = (void *)&((tap_dance_tap_hold_t){tap, hold, 0})}

// ----------------------------------------------------------------------------

// static td_tap_t tap_state[TDE_COUNT];
//
// static td_state_t cur_dance(tap_dance_state_t *state) {
//     if (state->count == 1) {
//         if (state->interrupted || !state->pressed) return TD_SINGLE_TAP;
//         else return TD_SINGLE_HOLD;
//     } else if (state->count == 2) {
//         if (state->interrupted) return TD_DOUBLE_SINGLE_TAP;
//         else if (state->pressed) return TD_DOUBLE_HOLD;
//         else return TD_DOUBLE_TAP;
//     }
//
//     if (state->count == 3) {
//         if (state->interrupted) return TD_TRIPLE_SINGLE_TAP;
//         else if (state->pressed) return TD_TRIPLE_HOLD;
//         else return TD_TRIPLE_TAP;
//     }
//
//     return TD_UNKNOWN;
// }

static void tap_dance_tap_hold_finished(tap_dance_state_t *state, void *user_data) {
    tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)user_data;

    if (state->pressed) {
        if (state->count == 1) {
            register_code16(tap_hold->hold);
            tap_hold->held = tap_hold->hold;
        } else {
            register_code16(tap_hold->tap);
            tap_hold->held = tap_hold->tap;
        }
    }
}

static void tap_dance_tap_hold_reset(tap_dance_state_t *state, void *user_data) {
    tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)user_data;

    if (tap_hold->held) {
        unregister_code16(tap_hold->held);
        tap_hold->held = 0;
    }
}

// ----------------------------------------------------------------------------

tap_dance_action_t user_tap_dance_actions[] = {
    [TDE_HRM_A] = ACTION_TAP_DANCE_TAP_HOLD(KC_A, KC_LGUI),
};

bool process_tap_dance_key(uint16_t keycode, keyrecord_t *record) {
    if (!is_user_td(keycode)) return true;
    if (record->event.pressed) return true;

    tap_dance_action_t *action;

    uint16_t idx = user_td_idx(keycode);
    switch (idx) {
        case TDE_HRM_A:
            action = tap_dance_get_user(idx);
            if (action->state.count && !action->state.finished) {
                tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)action->user_data;
                tap_code16(tap_hold->tap);
            }
    }

    return true;
}
