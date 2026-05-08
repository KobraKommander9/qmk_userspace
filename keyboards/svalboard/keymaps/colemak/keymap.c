#include <stdbool.h>
#include <stdint.h>

#include QMK_KEYBOARD_H
#include "quantum_keycodes.h"
#include "keycodes.h"
#include "modifiers.h"
#include "eeprom.h"

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
        /*RT*/ KC_ENTER          , CKC_SPC           , KC_ESC            , XXXXXXX         , MO(_SYS)          , XXXXXXX           ,
        /*LT*/ KC_BSPC           , KC_SPC            , HRM_TOGGLE        , KC_TAB          , CW_TOGG           , XXXXXXX
        ),

    [_NUM] = LAYOUT(
        /*     Center              North               East                South               West                Double*/
        /*R1*/ XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,
        /*R2*/ XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,
        /*R3*/ XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,
        /*R4*/ XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,

        /*L1*/ KC_KP_8,            KC_PPLS,            KC_KP_9,            KC_KP_4,            KC_PDOT,            XXXXXXX,
        /*L2*/ KC_KP_7,            KC_PMNS,            KC_RPRN,            KC_KP_3,            KC_LPRN,            XXXXXXX,
        /*L3*/ KC_KP_6,            KC_PAST,            KC_PEQL,            KC_KP_2,            KC_CIRC,            XXXXXXX,
        /*L4*/ KC_KP_5,            KC_PSLS,            KC_COMM,            KC_KP_1,            KC_KP_0,            XXXXXXX,

        /*     Down                Pad                 Up                  Nail                Knuckle             DoubleDown*/
        /*RT*/ XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,
        /*LT*/ KC_PENT,            KC_BSPC,            _______,            KC_SPC,             KC_TAB,             XXXXXXX
        ),

    [_SYS] = LAYOUT(
        /*     Center              North               East                South               West                Double*/
        /*R1*/ XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,
        /*R2*/ XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,
        /*R3*/ XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,
        /*R4*/ TO(_BASE),          XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,

        /*L1*/ XXXXXXX,            KC_BRIU,            XXXXXXX,            KC_BRID,            XXXXXXX,            XXXXXXX,
        /*L2*/ XXXXXXX,            SV_RDPU,            XXXXXXX,            SV_RDPD,            XXXXXXX,            XXXXXXX,
        /*L3*/ XXXXXXX,            SV_LDPU,            XXXXXXX,            SV_LDPD,            XXXXXXX,            XXXXXXX,
        /*L4*/ KC_PWR,             XXXXXXX,            XXXXXXX,            KC_SLEP,            XXXXXXX,            XXXXXXX,

        /*     Down                Pad                 Up                  Nail                Knuckle             DoubleDown*/
        /*RT*/ XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,
        /*LT*/ SV_SOUT,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX,            XXXXXXX
        ),

    [_MBO] = LAYOUT(
        /*     Center           North               East                South                West                Double*/
        /*R1*/ _______           , _______           , _______           , _______           , _______           , XXXXXXX             ,
        /*R2*/ _______           , _______           , _______           , _______           , _______           , XXXXXXX             ,
        /*R3*/ _______           , _______           , _______           , _______           , _______           , XXXXXXX             ,
        /*R4*/ _______           , _______           , _______           , _______           , _______           , XXXXXXX             ,

        /*L1*/ _______           , _______           , _______           , _______           , _______           , XXXXXXX             ,
        /*L2*/ _______           , _______           , _______           , _______           , _______           , XXXXXXX             ,
        /*L3*/ _______           , _______           , _______           , _______           , _______           , XXXXXXX             ,
        /*L4*/ _______           , _______           , _______           , _______           , _______           , XXXXXXX             ,

        /*     Down               Pad                Up                  Nail                Knuckle             DoubleDown*/
        /*RT*/ _______           , _______           , _______           , _______           , _______           , SV_SNIPER_5             ,
        /*LT*/ _______           , _______           , _______           , _______           , _______           , SV_SNIPER_3
        ),
};

void eeconfig_init_user(void) {
    init_hrm_eeprom();
}

void keyboard_post_init_user(void) {
    global_saved_values.left_scroll = true;
    global_saved_values.right_scroll = false;
    global_saved_values.auto_mouse = true;
    global_saved_values.left_dpi_index = MOUSE_DPI_400;
    global_saved_values.right_dpi_index = MOUSE_DPI_1200;
    global_saved_values.mh_timer_index = MOUSE_LAYER_TIMEOUT_NONE;

    setup_rgb_light_layer();
    setup_hrm_keys();
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!process_hrm(keycode, record)) return false;

    return true;
}

layer_state_t default_layer_state_set_user(layer_state_t state) {
    state = set_default_rgb_light(state);

    return state;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    state = set_rgb_light(state);

    // Disable auto-mouse when on the game layer
    // if (layer_state_cmp(state, _GAM1) || layer_state_cmp(state, _GAM2)) {
    //     global_saved_values.auto_mouse = false;
    //     mouse_mode(false);
    // } else {
    //     global_saved_values.auto_mouse = true;
    // }

    return state;
}
