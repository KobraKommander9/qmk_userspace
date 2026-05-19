#include "hrm.h"
#include "keys.h"
#include "persist.h"
#include "shift.h"

static void hrm_mode_cycle(void) {
    const hrm_mode_t mode = persist_get_hrm_mode();
    persist_set_hrm_last_mode(mode);
    persist_set_hrm_mode((mode + 1) % HRM_MODE_COUNT);
}

static void hrm_mode_toggle(void) {
    const hrm_mode_t mode = persist_get_hrm_mode();
    if (mode == HRM_MODE_OFF) {
        persist_set_hrm_mode(persist_get_hrm_last_mode());
    } else {
        persist_set_hrm_last_mode(mode);
        persist_set_hrm_mode(HRM_MODE_OFF);
    }
}

// ----------------------------------------------------------------------------

uint16_t get_hrm_tapping_term(uint16_t keycode, keyrecord_t* record) {
    switch (keycode) {
        case HRM_A:
        case HRM_O:
            return TAPPING_TERM + 5;

        case HRM_R:
        case HRM_I:
            return TAPPING_TERM + 10;

        case HRM_S:
        case HRM_E:
            return TAPPING_TERM + 15;

        case HRM_A:
        case HRM_O:
            return TAPPING_TERM + 5;
    }

    return TAPPING_TERM;
}

bool hrm_active(hrm_hand_t hand) {
    switch (persist_get_hrm_mode()) {
        case HRM_MODE_ON:    return true;
        case HRM_MODE_OFF:   return false;
        case HRM_MODE_LEFT:  return hand == HRM_HAND_LEFT;
        case HRM_MODE_RIGHT: return hand == HRM_HAND_RIGHT;
        default:             return false;
    }
}

void init_hrm_eeprom(void) {
    persist_reset();
}

bool is_hrm_keycode(uint16_t keycode) {
    switch (keycode) {
        case HRM_A:
        case HRM_R:
        case HRM_S:
        case HRM_T:

        case HRM_N:
        case HRM_E:
        case HRM_I:
        case HRM_O:
            return true;
    }

    return false;
}

bool process_hrm(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case HRM_A:
        case HRM_R:
        case HRM_S:
        case HRM_T: {
            if (!hrm_active(HRM_HAND_LEFT) && !record->tap.count && record->event.pressed) {
                uint16_t kc = QK_MOD_TAP_GET_TAP_KEYCODE(keycode);
                break_caps_word(kc);

                if (record->event.pressed) {
                    register_code(kc);
                } else {
                    unregister_code(kc);
                    mouse_mode(false);
                }

                return false;
            }

            break;
        }

        case HRM_N:
        case HRM_E:
        case HRM_I:
        case HRM_O: {
            if (!hrm_active(HRM_HAND_RIGHT) && !record->tap.count && record->event.pressed) {
                uint16_t kc = QK_MOD_TAP_GET_TAP_KEYCODE(keycode);
                break_caps_word(kc);

                if (record->event.pressed) {
                    register_code(kc);
                } else {
                    unregister_code(kc);
                    mouse_mode(false);
                }

                return false;
            }

            break;
        }

        case HRM_CYCLE:
            if (record->event.pressed) hrm_mode_cycle();
            return false;

        case HRM_TOGGLE:
            if (record->event.pressed) hrm_mode_toggle();
            return false;
    }

    return true;
}

void setup_hrm_keys(void) {
    persist_read();
}
