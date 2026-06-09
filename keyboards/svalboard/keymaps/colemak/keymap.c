#include <stdbool.h>
#include <stdint.h>

#include QMK_KEYBOARD_H
#include "quantum_keycodes.h"
#include "keycodes.h"
#include "modifiers.h"
#include "eeprom.h"
#include "svalboard.h"

#include "keys.h"
#include "hrm.h"
#include "layers.h"

const uint16_t PROGMEM keymaps[DYNAMIC_KEYMAP_LAYER_COUNT][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT(
        /*     Center            North               East                South             West                Double*/
        /*R1*/ HRM_N             , KC_L              , KC_MINS           , KC_H            , KC_M              , XXXXXXX           ,
        /*R2*/ HRM_E             , KC_U              , KC_EQL            , KC_COMMA        , KC_J              , XXXXXXX           ,
        /*R3*/ HRM_I             , KC_Y              , KC_EXLM           , KC_DOT          , KC_K              , XXXXXXX           ,
        /*R4*/ HRM_O             , KC_SCLN           , KC_RBRC           , KC_SLASH        , KC_RPRN           , XXXXXXX           ,

        /*L1*/ HRM_T             , KC_P              , KC_G              , KC_D            , KC_QUOT           , XXXXXXX           ,
        /*L2*/ HRM_S             , KC_F              , KC_B              , KC_C            , KC_GRV            , XXXXXXX           ,
        /*L3*/ HRM_R             , KC_W              , KC_V              , KC_X            , KC_BSLS           , XXXXXXX           ,
        /*L4*/ HRM_A             , KC_Q              , KC_LPRN           , KC_Z            , KC_LBRC           , XXXXXXX           ,

        /*     Down              Pad                 Up                  Nail              Knuckle             DoubleDown*/
        /*RT*/ KC_ENTER          , CKC_SPC           , KC_ESC            , CKC_MEDIA       , MO(_SYS)          , XXXXXXX           ,
        /*LT*/ CKC_TAB           , CKC_BSPC          , CW_TOGG           , CKC_GAME        , MO(_MBO)          , TO(_GAME1)
        ),

    [_GAME1] = LAYOUT(
        /*     Center              North               East                South               West                Double*/
        /*R1*/ KC_LEFT,            XXXXXXX,            XXXXXXX,            KC_1,               XXXXXXX,            XXXXXXX,
        /*R2*/ KC_DOWN,            KC_UP,              XXXXXXX,            KC_2,               XXXXXXX,            XXXXXXX,
        /*R3*/ KC_RIGHT,           XXXXXXX,            XXXXXXX,            KC_3,               XXXXXXX,            XXXXXXX,
        /*R4*/ XXXXXXX,            XXXXXXX,            XXXXXXX,            KC_4,               XXXXXXX,            XXXXXXX,

        /*L1*/ KC_D,               KC_E,               KC_V,               KC_F,               KC_G,               XXXXXXX,
        /*L2*/ KC_W,               KC_C,               KC_R,               KC_S,               KC_H,               XXXXXXX,
        /*L3*/ KC_A,               KC_Q,               KC_X,               KC_Z,               KC_B,               XXXXXXX,
        /*L4*/ KC_LSFT,            KC_TAB,             KC_LALT,            KC_LCTL,            KC_CAPS,            XXXXXXX,

        /*     Down                Pad                 Up                  Nail                Knuckle             DoubleDown*/
        /*RT*/ KC_LCTL,            KC_BTN1,            KC_BTN2,            XXXXXXX,            XXXXXXX,            TO(_BASE),
        /*LT*/ KC_BTN2,            KC_SPC,             KC_ESC,             SV_SNIPER_5,        MO(_GAME2),         TO(_BASE)
        ),

    [_GAME2] = LAYOUT(
        /*     Center              North               East                South               West                Double*/
        /*R1*/ XXXXXXX,            XXXXXXX,            XXXXXXX,            KC_F1,              XXXXXXX,            XXXXXXX,
        /*R2*/ XXXXXXX,            XXXXXXX,            XXXXXXX,            KC_F2,              XXXXXXX,            XXXXXXX,
        /*R3*/ XXXXXXX,            XXXXXXX,            XXXXXXX,            KC_F3,              XXXXXXX,            XXXXXXX,
        /*R4*/ XXXXXXX,            XXXXXXX,            XXXXXXX,            KC_F4,              XXXXXXX,            XXXXXXX,

        /*L1*/ KC_6,               KC_9,               KC_0,               KC_3,               XXXXXXX,            XXXXXXX,
        /*L2*/ KC_5,               KC_8,               XXXXXXX,            KC_2,               XXXXXXX,            XXXXXXX,
        /*L3*/ KC_4,               KC_7,               XXXXXXX,            KC_1,               XXXXXXX,            XXXXXXX,
        /*L4*/ KC_F5,              KC_F6,              KC_F8,              KC_F7,              KC_F9,              XXXXXXX,

        /*     Down                Pad                 Up                  Nail                Knuckle             DoubleDown*/
        /*RT*/ _______,            _______,            _______,            _______,            _______,            XXXXXXX,
        /*LT*/ _______,            XXXXXXX,            _______,            _______,            _______,            XXXXXXX
        ),

    [_NAV] = LAYOUT(
        /*     Center              North               East                South               West                Double*/
        /*R1*/ KC_LEFT,            KC_WREF,            _______,            _______,            _______,            XXXXXXX,
        /*R2*/ KC_DOWN,            KC_WBAK,            _______,            _______,            _______,            XXXXXXX,
        /*R3*/ KC_UP,              KC_WFWD,            _______,            _______,            _______,            XXXXXXX,
        /*R4*/ KC_RIGHT,           _______,            _______,            _______,            _______,            XXXXXXX,

        /*L1*/ _______,            _______,            _______,            _______,            _______,            XXXXXXX,
        /*L2*/ _______,            _______,            _______,            _______,            _______,            XXXXXXX,
        /*L3*/ _______,            _______,            _______,            _______,            _______,            XXXXXXX,
        /*L4*/ _______,            _______,            _______,            _______,            _______,            XXXXXXX,

        /*     Down                Pad                 Up                  Nail                Knuckle             DoubleDown*/
        /*RT*/ KC_LALT,            KC_TAB,             XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,
        /*LT*/ XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX
        ),

    [_NUM] = LAYOUT(
        /*     Center              North               East                South               West                Double*/
        /*R1*/ KC_F4,              KC_F7,              KC_0,               KC_F1,              _______,            XXXXXXX,
        /*R2*/ KC_F5,              KC_F8,              _______,            KC_F2,              _______,            XXXXXXX,
        /*R3*/ KC_F6,              KC_F9,              _______,            KC_F3,              _______,            XXXXXXX,
        /*R4*/ _______,            _______,            _______,            _______,            _______,            XXXXXXX,

        /*L1*/ KC_6,               KC_9,               KC_0,               KC_3,               KC_PEQL,            XXXXXXX,
        /*L2*/ KC_5,               KC_8,               XXXXXXX,            KC_2,               KC_PSLS,            XXXXXXX,
        /*L3*/ KC_4,               KC_7,               XXXXXXX,            KC_1,               KC_PCMM,            XXXXXXX,
        /*L4*/ KC_PENT,            KC_PPLS,            KC_PAST,            KC_PMNS,            KC_DOT,             XXXXXXX,

        /*     Down                Pad                 Up                  Nail                Knuckle             DoubleDown*/
        /*RT*/ _______,            _______,            KC_LCTL,            KC_LALT,            XXXXXXX,            XXXXXXX,
        /*LT*/ KC_PENT,            KC_BSPC,            _______,            KC_SPC,             KC_TAB,             XXXXXXX
        ),

    [_SYM] = LAYOUT(
        /*     Center              North               East                South               West                Double*/
        /*R1*/ KC_LCBR,            KC_LT,              KC_DLR,             KC_LBRC,            KC_CIRC,            _______,
        /*R2*/ KC_COLN,            KC_EQUAL,           KC_PIPE,            KC_ASTR,            KC_AMPR,            _______,
        /*R3*/ KC_RCBR,            KC_GT,              KC_PERC,            KC_RBRC,            KC_EXLM,            _______,
        /*R4*/ KC_AT,              KC_UNDS,            KC_BSLS,            KC_HASH,            KC_SLSH,            _______,

        /*L1*/ _______,            _______,            _______,            _______,            _______,            _______,
        /*L2*/ _______,            _______,            _______,            _______,            _______,            _______,
        /*L3*/ _______,            _______,            _______,            _______,            _______,            _______,
        /*L4*/ _______,            _______,            _______,            _______,            _______,            _______,

        /*     Down                Pad                 Up                  Nail                Knuckle             DoubleDown*/
        /*RT*/ _______,            KC_SPC,             QK_REP,             KC_BSPC,            KC_TAB,             XXXXXXX,
        /*LT*/ XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX
        ),

    [_MEDIA] = LAYOUT(
        /*     Center              North               East                South               West                Double*/
        /*R1*/ M_NEXT,             M_VOL_DN,           XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,
        /*R2*/ M_PREV,             M_VOL_UP,           XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,
        /*R3*/ M_PLAY,             XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,
        /*R4*/ M_STOP,             XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,

        /*L1*/ XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,
        /*L2*/ XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,
        /*L3*/ XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,
        /*L4*/ XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,

        /*     Down                Pad                 Up                  Nail                Knuckle             DoubleDown*/
        /*RT*/ XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            TO(_BASE),
        /*LT*/ XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            TO(_BASE)
        ),

    [_SYS] = LAYOUT(
        /*     Center              North               East                South               West                Double*/
        /*R1*/ XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,
        /*R2*/ XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,
        /*R3*/ XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,
        /*R4*/ XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,

        /*L1*/ XXXXXXX,            KC_BRIU,            XXXXXXX,            KC_BRID,            XXXXXXX,            XXXXXXX,
        /*L2*/ XXXXXXX,            SV_RDPU,            XXXXXXX,            SV_RDPD,            XXXXXXX,            XXXXXXX,
        /*L3*/ XXXXXXX,            SV_LDPU,            XXXXXXX,            SV_LDPD,            XXXXXXX,            XXXXXXX,
        /*L4*/ KC_PWR,             XXXXXXX,            XXXXXXX,            KC_SLEP,            XXXXXXX,            XXXXXXX,

        /*     Down                Pad                 Up                  Nail                Knuckle             DoubleDown*/
        /*RT*/ XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            TO(_BASE),
        /*LT*/ SV_SOUT,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            TO(_BASE)
        ),

    [_MBO] = LAYOUT(
        /*     Center           North               East                South                West                Double*/
        /*R1*/ KC_BTN3           , _______           , _______           , _______           , _______           , _______             ,
        /*R2*/ KC_BTN4           , _______           , _______           , _______           , _______           , _______             ,
        /*R3*/ KC_BTN5           , _______           , _______           , _______           , _______           , _______             ,
        /*R4*/ _______           , _______           , _______           , _______           , _______           , _______             ,

        /*L1*/ _______           , _______           , _______           , _______           , _______           , _______             ,
        /*L2*/ _______           , _______           , _______           , _______           , _______           , _______             ,
        /*L3*/ _______           , _______           , _______           , _______           , _______           , _______             ,
        /*L4*/ _______           , _______           , _______           , _______           , _______           , _______             ,

        /*     Down               Pad                Up                  Nail                Knuckle             DoubleDown*/
        /*RT*/ KC_BTN2           , KC_BTN1           , KC_BTN2           , _______           , _______           , SV_SNIPER_5         ,
        /*LT*/ KC_BTN2           , KC_BTN1           , _______           , SV_SNIPER_3       , _______           , SV_SNIPER_5
        ),

    // [_TMP] = LAYOUT(
    //     /*     Center              North               East                South               West                Double*/
    //     /*R1*/ XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,
    //     /*R2*/ XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,
    //     /*R3*/ XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,
    //     /*R4*/ XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,
    //
    //     /*L1*/ XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,
    //     /*L2*/ XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,
    //     /*L3*/ XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,
    //     /*L4*/ XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,
    //
    //     /*     Down                Pad                 Up                  Nail                Knuckle             DoubleDown*/
    //     /*RT*/ XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,
    //     /*LT*/ XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX
    //     ),
};

void eeconfig_init_user(void) {
    init_hrm_eeprom();
}

void keyboard_post_init_user(void) {
    global_saved_values.left_scroll = true;
    global_saved_values.right_scroll = false;
    global_saved_values.auto_mouse = true;
    global_saved_values.left_dpi_index = MOUSE_DPI_600;
    global_saved_values.right_dpi_index = MOUSE_DPI_600;
    global_saved_values.mh_timer_index = MOUSE_LAYER_TIMEOUT_NONE;

    setup_hrm_keys();
    setup_rgb_colors();
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!process_hrm(keycode, record)) return false;

    return true;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    if (layer_state_cmp(state, _GAME1) || layer_state_cmp(state, _GAME2)) {
        global_saved_values.auto_mouse = false;
        mouse_mode(false);
    } else {
        global_saved_values.auto_mouse = true;
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
