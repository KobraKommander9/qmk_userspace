#include "tap_dance.h"
#include "keys.h"
#include "layers.h"

static td_user_state_t dance_state[TD_COUNT];

bool process_custom_dances(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case CKC_SPC: record->keycode = TD(_TD_CKC_SPC);
    }

    return true;
}
static void ckc_spc_finished(tap_dance_state_t *state, void *user_data) {
    dance_state[_TD_CKC_SPC].state = *state;
    if (state->pressed) {
        layer_on(_NUM);
    } else {
        tap_code(KC_SPC);
    }
}

static void ckc_spc_reset(tap_dance_state_t *state, void *user_data) {
    if (dance_state[_TD_CKC_SPC].state.pressed) {
        layer_off(_NUM);
    }
}

tap_dance_action_t tap_dance_actions[] = {
  [_TD_CKC_SPC] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, ckc_spc_finished, ckc_spc_reset),
};
