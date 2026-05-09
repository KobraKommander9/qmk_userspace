#include "hrm.h"
#include "keys.h"
#include "persist.h"

void init_hrm_eeprom(void) {
    persist_reset();
}

void setup_hrm_keys(void) {
    persist_read();
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

bool process_hrm(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case HRM_CYCLE:
            if (record->event.pressed) hrm_mode_cycle();
            return false;

        case HRM_TOGGLE:
            if (record->event.pressed) hrm_mode_toggle();
            return false;
    }

    return true;
}
