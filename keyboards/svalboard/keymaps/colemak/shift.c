#include "shift.h"

void break_caps_word(uint16_t kc) {
    if (!is_caps_word_on()) return;

    switch (kc) {
        case KC_A ... KC_Z:
        case KC_1 ... KC_0:
        case KC_MINS:
        case KC_BSPC:
        case KC_DEL:
        case KC_UNDS:
            break;

        default:
            caps_word_off();
    }
}

shift_state_t get_shift_state(void) {
    bool phys = (get_mods() & MOD_MASK_SHIFT);
    bool caps  = is_caps_word_on() || host_keyboard_led_state().caps_lock;

    return (shift_state_t){
        .phys = phys,
        .caps = caps,
        .effective = phys ^ caps,
    };
}
