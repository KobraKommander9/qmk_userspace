#include <stdbool.h>
#include <stdint.h>

#include QMK_KEYBOARD_H
#include "quantum_keycodes.h"
#include "keycodes.h"
#include "modifiers.h"
#include "eeprom.h"
#include "svalboard.h"

#include "hrm.h"
#include "keys.h"
#include "layers.h"
#include "macros.h"

const uint16_t PROGMEM keymaps[DYNAMIC_KEYMAP_LAYER_COUNT][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT(
        /*     Center           North               East                South               West                Double*/
        /*R1*/ HRM_N,           KC_L,               KC_MINS,            KC_M,               KC_H,               XXXXXXX,
        /*R2*/ HRM_E,           KC_U,               KC_EQL,             KC_COMMA,           KC_J,               XXXXXXX,
        /*R3*/ HRM_I,           KC_Y,               KC_EXLM,            KC_DOT,             KC_K,               XXXXXXX,
        /*R4*/ HRM_O,           KC_SCLN,            KC_RBRC,            KC_SLSH,            KC_RPRN,            XXXXXXX,

        /*L1*/ HRM_T,           KC_P,               KC_G,               KC_D,               KC_QUOT,            XXXXXXX,
        /*L2*/ HRM_S,           KC_F,               KC_B,               KC_C,               KC_GRV,             XXXXXXX,
        /*L3*/ HRM_R,           KC_W,               KC_V,               KC_X,               KC_BSLS,            XXXXXXX,
        /*L4*/ HRM_A,           KC_Q,               KC_LPRN,            KC_Z,               KC_LBRC,            XXXXXXX,

        /*     Down             Pad                 Up                  Nail                Knuckle             DoubleDown*/
        /*RT*/ LT_ENT,          KC_SPC,             KC_ESC,             MO(_NAV),           KC_HYPR,            XXXXXXX,
        /*LT*/ LT_TAB,          KC_BSPC,            CW_TOGG,            CKC_GAME,           KC_MEH,             XXXXXXX
    ),

    [_GAME1] = LAYOUT(
        /*     Center           North               East                South               West                Double*/
        /*R1*/ KC_LEFT,         XXXXXXX,            XXXXXXX,            KC_1,               XXXXXXX,            XXXXXXX,
        /*R2*/ KC_DOWN,         KC_UP,              XXXXXXX,            KC_2,               XXXXXXX,            XXXXXXX,
        /*R3*/ KC_RIGHT,        XXXXXXX,            XXXXXXX,            KC_3,               XXXXXXX,            XXXXXXX,
        /*R4*/ XXXXXXX,         XXXXXXX,            XXXXXXX,            KC_4,               XXXXXXX,            XXXXXXX,

        /*L1*/ KC_D,            KC_E,               KC_V,               KC_F,               KC_G,               XXXXXXX,
        /*L2*/ KC_W,            KC_C,               KC_R,               KC_S,               KC_H,               XXXXXXX,
        /*L3*/ KC_A,            KC_Q,               KC_X,               KC_Z,               KC_B,               XXXXXXX,
        /*L4*/ KC_SPC,          KC_TAB,             MS_BTN2,            KC_LCTL,            KC_LSFT,            XXXXXXX,

        /*     Down             Pad                 Up                  Nail                Knuckle             DoubleDown*/
        /*RT*/ KC_LCTL,         XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            TO(_BASE),
        /*LT*/ MS_BTN1,         XXXXXXX,            KC_ESC,             SV_SNIPER_5,        MO(_GAME2),         TO(_BASE)
    ),

    [_GAME2] = LAYOUT(
        /*     Center           North               East                South               West                Double*/
        /*R1*/ XXXXXXX,         XXXXXXX,            XXXXXXX,            KC_F1,              XXXXXXX,            XXXXXXX,
        /*R2*/ XXXXXXX,         XXXXXXX,            XXXXXXX,            KC_F2,              XXXXXXX,            XXXXXXX,
        /*R3*/ XXXXXXX,         XXXXXXX,            XXXXXXX,            KC_F3,              XXXXXXX,            XXXXXXX,
        /*R4*/ XXXXXXX,         XXXXXXX,            XXXXXXX,            KC_F4,              XXXXXXX,            XXXXXXX,

        /*L1*/ KC_6,            KC_9,               KC_0,               KC_3,               XXXXXXX,            XXXXXXX,
        /*L2*/ KC_5,            KC_8,               XXXXXXX,            KC_2,               XXXXXXX,            XXXXXXX,
        /*L3*/ KC_4,            KC_7,               KC_LALT,            KC_1,               XXXXXXX,            XXXXXXX,
        /*L4*/ KC_F5,           KC_F6,              KC_F8,              KC_F7,              KC_F9,              XXXXXXX,

        /*     Down             Pad                 Up                  Nail                Knuckle             DoubleDown*/
        /*RT*/ _______,         _______,            _______,            _______,            _______,            XXXXXXX,
        /*LT*/ _______,         XXXXXXX,            _______,            _______,            _______,            XXXXXXX
    ),

    [_SYM] = LAYOUT(
        /*     Center           North               East                South               West                Double*/
        /*R1*/ M_WAL,           KC_ASTR,            KC_DLR,             M_ARR,              KC_CIRC,            XXXXXXX,
        /*R2*/ M_AND,           KC_HASH,            KC_AMPR,            M_FAT,              KC_PIPE,            XXXXXXX,
        /*R3*/ M_OR,            KC_BSLS,            XXXXXXX,            M_SCP,              XXXXXXX,            XXXXXXX,
        /*R4*/ KC_AT,           KC_PERC,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,

        /*L1*/ _______,         _______,            _______,            _______,            _______,            XXXXXXX,
        /*L2*/ _______,         _______,            _______,            _______,            _______,            XXXXXXX,
        /*L3*/ _______,         _______,            _______,            _______,            _______,            XXXXXXX,
        /*L4*/ _______,         _______,            _______,            _______,            _______,            XXXXXXX,

        /*     Down             Pad                 Up                  Nail                Knuckle             DoubleDown*/
        /*RT*/ _______,         _______,            _______,            _______,            _______,            _______,
        /*LT*/ _______,         _______,            _______,            _______,            _______,            QK_LLCK
    ),


    [_NUM] = LAYOUT(
        /*     Center           North               East                South               West                Double*/
        /*R1*/ KC_F5,           KC_F1,              XXXXXXX,            KC_F9,              XXXXXXX,            XXXXXXX,
        /*R2*/ KC_F6,           KC_F2,              XXXXXXX,            KC_F10,             XXXXXXX,            XXXXXXX,
        /*R3*/ KC_F7,           KC_F3,              XXXXXXX,            KC_F11,             XXXXXXX,            XXXXXXX,
        /*R4*/ KC_F8,           KC_F4,              XXXXXXX,            KC_F12,             XXXXXXX,            XXXXXXX,

        /*L1*/ KC_6,            KC_9,               KC_0,               KC_3,               XXXXXXX,            XXXXXXX,
        /*L2*/ KC_5,            KC_8,               XXXXXXX,            KC_2,               XXXXXXX,            XXXXXXX,
        /*L3*/ KC_4,            KC_7,               XXXXXXX,            KC_1,               XXXXXXX,            XXXXXXX,
        /*L4*/ XXXXXXX,         XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,

        /*     Down             Pad                 Up                  Nail                Knuckle             DoubleDown*/
        /*RT*/ _______,         _______,            _______,            _______,            _______,            QK_LLCK,
        /*LT*/ _______,         _______,            _______,            _______,            _______,            _______
    ),

    [_NAV] = LAYOUT(
        /*     Center           North               East                South               West                Double*/
        /*R1*/ KC_DOWN,         KC_HOME,            _______,            LGUI(KC_X),         KC_LEFT,            XXXXXXX,
        /*R2*/ KC_UP,           KC_PGDN,            _______,            LGUI(KC_C),         _______,            XXXXXXX,
        /*R3*/ KC_RIGHT,        KC_PGUP,            _______,            LGUI(KC_V),         _______,            XXXXXXX,
        /*R4*/ KC_ESC,          KC_END,             _______,            _______,            _______,            XXXXXXX,

        /*L1*/ KC_LCTL,         KC_WREF,            _______,            LCTL(KC_V),         _______,            XXXXXXX,
        /*L2*/ KC_LSFT,         KC_WBAK,            _______,            LCTL(KC_C),         _______,            XXXXXXX,
        /*L3*/ KC_LALT,         KC_WFWD,            _______,            LCTL(KC_X),         _______,            XXXXXXX,
        /*L4*/ KC_LGUI,         _______,            _______,            TG(_MEDIA),         _______,            XXXXXXX,

        /*     Down             Pad                 Up                  Nail                Knuckle             DoubleDown*/
        /*RT*/ _______,         _______,            _______,            _______,            _______,            _______,
        /*LT*/ _______,         _______,            _______,            _______,            _______,            _______
    ),

    [_MEDIA] = LAYOUT(
        /*     Center           North               East                South               West                Double*/
        /*R1*/ M_NEXT,          M_VOL_DN,           XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,
        /*R2*/ M_PREV,          M_VOL_UP,           XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,
        /*R3*/ M_PLAY,          XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,
        /*R4*/ M_STOP,          XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,

        /*L1*/ XXXXXXX,         XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,
        /*L2*/ XXXXXXX,         XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,
        /*L3*/ XXXXXXX,         XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,
        /*L4*/ XXXXXXX,         XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,

        /*     Down             Pad                 Up                  Nail                Knuckle             DoubleDown*/
        /*RT*/ XXXXXXX,         XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            TO(_BASE),
        /*LT*/ XXXXXXX,         XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            TO(_BASE)
    ),

    [_SYS] = LAYOUT(
        /*     Center           North               East                South               West                Double*/
        /*R1*/ XXXXXXX,         XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,
        /*R2*/ XXXXXXX,         XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,
        /*R3*/ XXXXXXX,         XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,
        /*R4*/ XXXXXXX,         XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,

        /*L1*/ XXXXXXX,         KC_BRIU,            XXXXXXX,            KC_BRID,            XXXXXXX,            XXXXXXX,
        /*L2*/ XXXXXXX,         SV_RDPU,            XXXXXXX,            SV_RDPD,            XXXXXXX,            XXXXXXX,
        /*L3*/ XXXXXXX,         SV_LDPU,            XXXXXXX,            SV_LDPD,            XXXXXXX,            XXXXXXX,
        /*L4*/ KC_PWR,          XXXXXXX,            XXXXXXX,            KC_SLEP,            XXXXXXX,            XXXXXXX,

        /*     Down             Pad                 Up                  Nail                Knuckle             DoubleDown*/
        /*RT*/ XXXXXXX,         XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            TO(_BASE),
        /*LT*/ SV_SOUT,         XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            TO(_BASE)
    ),

    [_MBO] = LAYOUT(
        /*     Center           North               East                South               West                Double*/
        /*R1*/ _______,         _______,            _______,            _______,            _______,            _______,
        /*R2*/ _______,         _______,            _______,            _______,            _______,            _______,
        /*R3*/ _______,         _______,            _______,            _______,            _______,            _______,
        /*R4*/ _______,         _______,            _______,            _______,            _______,            _______,

        /*L1*/ _______,         _______,            _______,            _______,            _______,            _______,
        /*L2*/ _______,         _______,            _______,            _______,            _______,            _______,
        /*L3*/ _______,         _______,            _______,            _______,            _______,            _______,
        /*L4*/ _______,         _______,            _______,            _______,            _______,            _______,

        /*     Down             Pad                 Up                  Nail                Knuckle             DoubleDown*/
        /*RT*/ MS_BTN1,         _______,            MS_BTN2,            MS_BTN3,            SV_SNIPER_3,        _______,
        /*LT*/ MS_BTN1,         MS_BTN4,            MS_BTN2,            MS_BTN3,            SV_SNIPER_3,        _______
    ),

    // [_TMP] = LAYOUT(
    //     /*     Center           North               East                South               West                Double*/
    //     /*R1*/ XXXXXXX,         XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,
    //     /*R2*/ XXXXXXX,         XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,
    //     /*R3*/ XXXXXXX,         XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,
    //     /*R4*/ XXXXXXX,         XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,
    //
    //     /*L1*/ XXXXXXX,         XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,
    //     /*L2*/ XXXXXXX,         XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,
    //     /*L3*/ XXXXXXX,         XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,
    //     /*L4*/ XXXXXXX,         XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,
    //
    //     /*     Down             Pad                 Up                  Nail                Knuckle             DoubleDown*/
    //     /*RT*/ XXXXXXX,         XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,
    //     /*LT*/ XXXXXXX,         XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX
    // ),
};

// The kb resets its saved values after eeconfig_init_user runs, so ours are
// applied in keyboard_post_init_user instead.
static bool apply_sval_defaults = false;

void eeconfig_init_user(void) {
    init_hrm_eeprom();
    apply_sval_defaults = true;
}

void keyboard_post_init_user(void) {
    if (apply_sval_defaults) {
        global_saved_values.left_scroll = true;
        global_saved_values.right_scroll = false;
        global_saved_values.auto_mouse = true;
        global_saved_values.left_dpi_index = MOUSE_DPI_200;
        global_saved_values.right_dpi_index = MOUSE_DPI_800;
        global_saved_values.mh_timer_index = MOUSE_LAYER_TIMEOUT_200;

        write_eeprom_kb();
        set_dpi_from_eeprom();
    }

    setup_hrm_keys();
    setup_rgb_colors();
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!process_hrm(keycode, record)) return false;
    if (!process_macro(keycode, record)) return false;

    return true;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    static bool gaming = false;
    static bool saved_auto_mouse;
    bool game_layer = layer_state_cmp(state, _GAME1) || layer_state_cmp(state, _GAME2);

    // set_right_dpi doesn't persist, so leaving restores the saved DPI.
    if (game_layer != gaming) {
        gaming = game_layer;
        if (gaming) {
            saved_auto_mouse = global_saved_values.auto_mouse;
            // mouse_mode is a no-op once auto_mouse is off, and its nested
            // layer_off is overwritten by the state returned here.
            mouse_mode(false);
            state &= ~((layer_state_t)1 << MH_AUTO_BUTTONS_LAYER);
            global_saved_values.auto_mouse = false;
            set_right_dpi(MOUSE_DPI_800);
        } else {
            global_saved_values.auto_mouse = saved_auto_mouse;
            // Any kb EEPROM write while gaming persisted auto_mouse = false.
            write_eeprom_kb();
            set_dpi_from_eeprom();
        }
    }

    return state;
}

bool caps_word_press_user(uint16_t keycode) {
    switch (keycode) {
        // Keycodes that continue Caps Word, with shift applied.
        case KC_A ... KC_Z:
        case KC_MINS:
            add_weak_mods(MOD_BIT(KC_LSFT)); // Apply shift to next key.
            return true;

        // Keycodes that continue Caps Word, without shifting.
        case KC_1 ... KC_0:
        case KC_BSPC:
        case KC_DEL:
        case KC_UNDS:
            return true;

        // allow tap dances to determine caps word continuation.
        case QK_TAP_DANCE ... QK_TAP_DANCE_MAX:
            return true;

        default:
            return false; // Deactivate Caps Word.
    }
}

bool has_user_tapping_term(uint16_t keycode, keyrecord_t *record) {
    return is_hrm_keycode(keycode);
}

uint16_t get_tapping_term_user(uint16_t keycode, keyrecord_t *record) {
    if (is_hrm_keycode(keycode)) {
        return get_hrm_tapping_term(keycode, record);
    }

    return TAPPING_TERM;
}
