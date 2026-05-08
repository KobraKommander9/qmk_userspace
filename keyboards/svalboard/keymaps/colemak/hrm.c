#include "hrm.h"
#include "keys.h"
#include "eeprom.h"

// Pick an address in the user EEPROM space
#define EEPROM_HRM_MODE offsetof(user_config_t, hrm_mode)

typedef union {
    uint32_t raw;
    struct {
        hrm_mode_t hrm_mode : 3;
        hrm_mode_t hrm_last_mode : 3;
    };
} user_config_t;

static user_config_t user_config;

void init_hrm_eeprom() {
    user_config.raw = 0;
    user_config.hrm_mode = HRM_MODE_ON;
    user_config.hrm_last_mode = user_config.hrm_mode;
    eeconfig_update_user(user_config.raw);
}

void setup_hrm_keys() {
    user_config.raw = eeconfig_read_user();
    if (user_config.hrm_mode >= HRM_MODE_COUNT) {
        user_config.hrm_mode = HRM_MODE_ON;
    }
}

static bool hrm_active(hrm_hand_t hand) {
    switch (user_config.hrm_mode) {
        case HRM_MODE_ON: return true;
        case HRM_MODE_OFF: return false;
        case HRM_MODE_LEFT: return hand == HRM_HAND_LEFT;
        case HRM_MODE_RIGHT: return hand == HRM_HAND_RIGHT;
        default: return false;
    }
}

static void hrm_mode_cycle(void) {
    user_config.hrm_last_mode = user_config.hrm_mode;
    user_config.hrm_mode = (user_config.hrm_mode + 1) % HRM_MODE_COUNT;

    eeconfig_update_user(user_config.raw);
}

static void hrm_mode_toggle(void) {
    if (user_config.hrm_mode == HRM_MODE_OFF) {
        user_config.hrm_mode = user_config.hrm_last_mode;
    } else {
        user_config.hrm_last_mode = user_config.hrm_mode;
        user_config.hrm_mode = HRM_MODE_OFF;
    }

    eeconfig_update_user(user_config.raw);
}

#define HANDLE_HRM(tap_kc, mod_kc, hand) \
    if (!hrm_active(hand)) { \
        if (record->event.pressed) register_code(tap_kc); \
        else unregister_code(tap_kc); \
        return false; \
    } \
    record->keycode = mod_kc; \
    return true;

bool process_hrm(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case HRM_CYCLE:
            if (record->event.pressed) hrm_mode_cycle();
            return false;

        case HRM_TOGGLE:
            if (record->event.pressed) hrm_mode_toggle();
            return false;

        case HRM_A: HANDLE_HRM(KC_A, LGUI_T(KC_A), HRM_HAND_LEFT);
        case HRM_R: HANDLE_HRM(KC_R, LALT_T(KC_R), HRM_HAND_LEFT);
        case HRM_S: HANDLE_HRM(KC_S, LSFT_T(KC_S), HRM_HAND_LEFT);
        case HRM_T: HANDLE_HRM(KC_T, LCTL_T(KC_T), HRM_HAND_LEFT);

        case HRM_N: HANDLE_HRM(KC_N, RCTL_T(KC_N), HRM_HAND_RIGHT);
        case HRM_E: HANDLE_HRM(KC_E, RSFT_T(KC_E), HRM_HAND_RIGHT);
        case HRM_I: HANDLE_HRM(KC_I, LALT_T(KC_I), HRM_HAND_RIGHT);
        case HRM_O: HANDLE_HRM(KC_O, RGUI_T(KC_O), HRM_HAND_RIGHT);
    }

    return true;
}
