#include "custom_keys.h"

#include "keys.h"

// The key chosen on press, so the release matches even if Shift changed in between.
static uint16_t held_bspc;
static uint16_t held_exlm;

static void press_bspc(void) {
    uint8_t shift = get_mods() & MOD_MASK_SHIFT;
    if (!shift) {
        held_bspc = KC_BSPC;
        register_code(held_bspc);
        return;
    }

    // Shift+Delete is its own shortcut on most hosts; send a plain Delete.
    held_bspc = KC_DEL;
    del_mods(shift);
    register_code(held_bspc);
    add_mods(shift);
}

bool process_custom_key(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case CK_BSPC:
            if (record->event.pressed) {
                press_bspc();
            } else {
                unregister_code(held_bspc);
            }
            return false;

        case CK_EXLM:
            if (record->event.pressed) {
                held_exlm = (get_mods() & MOD_MASK_SHIFT) ? KC_AMPR : KC_EXLM;
                register_code16(held_exlm);
            } else {
                unregister_code16(held_exlm);
            }
            return false;
    }

    return true;
}
