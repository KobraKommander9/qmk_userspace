#include "tap_dance.h"
#include "hrm.h"

#include <stdint.h>

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

typedef bool (*tap_dance_enabled_fn_t)(void *ctx);

typedef struct {
    uint16_t held;
    uint16_t hold;
    uint16_t tap;

    tap_dance_enabled_fn_t is_enabled_fn;
    void *enabled_ctx;
} tap_dance_tap_hold_t;

// ----------------------------------------------------------------------------

static bool is_hrm_enabled(void *ctx) {
    hrm_hand_t hand = (hrm_hand_t)(uintptr_t)ctx;
    return hrm_active(hand);
}

// ----------------------------------------------------------------------------

#define OVERLOAD4(_1, _2, _3, _4, NAME, ...) NAME
#define ACTION_TAP_DANCE_TAP_HOLD(...)  \
    OVERLOAD4(__VA_ARGS__,              \
        ACTION_TAP_DANCE_TAP_HOLD4,     \
        INVALID,                        \
        ACTION_TAP_DANCE_TAP_HOLD2      \
    )(__VA_ARGS__)
#define ACTION_TAP_DANCE_TAP_HOLD2(...) \
    ACTION_TAP_DANCE_TAP_HOLD4(__VA_ARGS__, NULL, NULL)
#define ACTION_TAP_DANCE_TAP_HOLD4(tap_kc, hold_kc, enabled_fn, ctx)        \
    {.fn = {NULL, tap_dance_tap_hold_finished, tap_dance_tap_hold_reset},   \
        .user_data = (void *)&((tap_dance_tap_hold_t){                      \
            .tap = (tap_kc),                                                \
            .hold = (hold_kc),                                              \
            .held = 0,                                                      \
            .is_enabled_fn = (enabled_fn),                                  \
            .enabled_ctx = (ctx),                                           \
        })}
#define ACTION_TAP_DANCE_HRM(tap, hold, hand) \
    ACTION_TAP_DANCE_TAP_HOLD(tap, hold, is_hrm_enabled, (void *)(uintptr_t)(hand))

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
    if (!state->pressed) return;

    tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)user_data;

    bool enabled = tap_hold->is_enabled_fn == NULL ||
        tap_hold->is_enabled_fn(tap_hold->enabled_ctx);

    if (enabled && state->count == 1) {
        register_code16(tap_hold->hold);
        tap_hold->held = tap_hold->hold;
    } else {
        register_code16(tap_hold->tap);
        tap_hold->held = tap_hold->tap;
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
    [TDE_HRM_A] = ACTION_TAP_DANCE_HRM(KC_A, KC_LGUI, HRM_HAND_LEFT),
    [TDE_HRM_R] = ACTION_TAP_DANCE_HRM(KC_R, KC_LALT, HRM_HAND_LEFT),
    [TDE_HRM_S] = ACTION_TAP_DANCE_HRM(KC_S, KC_LSFT, HRM_HAND_LEFT),
    [TDE_HRM_T] = ACTION_TAP_DANCE_HRM(KC_T, KC_LCTL, HRM_HAND_LEFT),

    [TDE_HRM_N] = ACTION_TAP_DANCE_HRM(KC_N, KC_RCTL, HRM_HAND_RIGHT),
    [TDE_HRM_E] = ACTION_TAP_DANCE_HRM(KC_E, KC_RSFT, HRM_HAND_RIGHT),
    [TDE_HRM_I] = ACTION_TAP_DANCE_HRM(KC_I, KC_LALT, HRM_HAND_RIGHT),
    [TDE_HRM_O] = ACTION_TAP_DANCE_HRM(KC_O, KC_RGUI, HRM_HAND_RIGHT),
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
