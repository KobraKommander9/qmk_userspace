#include <stdbool.h>
#include <stdint.h>

#include QMK_KEYBOARD_H
#include "quantum_keycodes.h"
#include "keycodes.h"
#include "modifiers.h"

#include "keys.h"
#include "layers.h"

const uint16_t PROGMEM keymaps[DYNAMIC_KEYMAP_LAYER_COUNT][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT(
        /*     Center            North               East                South             West                Double*/
        /*R1*/ HOME_N            , KC_L              , KC_MINS           , KC_H            , KC_M              , XXXXXXX           ,
        /*R2*/ HOME_E            , KC_U              , KC_EQL            , KC_COMMA        , KC_J              , XXXXXXX           ,
        /*R3*/ HOME_I            , KC_Y              , KC_EXLM           , KC_DOT          , KC_K              , XXXXXXX           ,
        /*R4*/ HOME_O            , KC_SCLN           , KC_RBRC           , KC_SLASH        , KC_RPRN           , XXXXXXX           ,

        /*L1*/ HOME_T            , KC_P              , KC_G              , KC_D            , KC_QUOT           , XXXXXXX           ,
        /*L2*/ HOME_S            , KC_F              , KC_B              , KC_C            , KC_GRV            , XXXXXXX           ,
        /*L3*/ HOME_R            , KC_W              , KC_V              , KC_X            , KC_BSLS           , XXXXXXX           ,
        /*L4*/ HOME_A            , KC_Q              , KC_LPRN           , KC_Z            , KC_LBRC           , XXXXXXX           ,

        /*     Down              Pad                 Up                  Nail              Knuckle             DoubleDown*/
        /*RT*/ KC_ENTER          , KC_SPC            , KC_ESC            , XXXXXXX         , XXXXXXX           , XXXXXXX           ,
        /*LT*/ KC_BSPC           , KC_SPC            , XXXXXXX           , KC_TAB          , CW_TOGG           , XXXXXXX
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

void keyboard_post_init_user(void) {
    global_saved_values.left_scroll = true;
    global_saved_values.right_scroll = false;
    global_saved_values.auto_mouse = true;
    global_saved_values.left_dpi_index = MOUSE_DPI_400;
    global_saved_values.right_dpi_index = MOUSE_DPI_1200;
    global_saved_values.mh_timer_index = MOUSE_LAYER_TIMEOUT_NONE;

    setup_rgb_light_layer();
}
