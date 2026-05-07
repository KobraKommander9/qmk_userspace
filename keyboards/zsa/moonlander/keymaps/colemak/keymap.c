#include QMK_KEYBOARD_H
#include "version.h"
#include "combos.h"
#include "keys.h"
#include "layers.h"
#include "rgb.h"
#include "tap_dance.h"

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [CLMK] = LAYOUT_moonlander(
        KC_EQUAL,   KC_1,       KC_2,       KC_3,       KC_4,       KC_5,       KC_LEFT,        KC_RIGHT,   KC_6,       KC_7,       KC_8,       KC_9,       KC_0,       KC_MINUS,
        CW_TOGG,    KC_Q,       KC_W,       KC_F,       KC_P,       KC_G,       TO(SYMB),       KC_CAPS,    KC_J,       KC_L,       KC_U,       KC_Y,       KC_SCLN,    KC_BSLS,
        KC_TAB,     HOME_A,     HOME_R,     HOME_S,     HOME_T,     KC_D,       KC_HYPR,        KC_MEH,     KC_H,       HOME_N,     HOME_E,     HOME_I,     HOME_O,     KC_QUOTE,
        KC_LSFT,    KC_Z,       KC_X,       KC_C,       TD0,        KC_B,                                   KC_K,       KC_M,       KC_COMM,    KC_DOT,     KC_SLASH,   KC_RSFT,
        KC_LALT,    KC_LEFT,    KC_RIGHT,   KC_LGUI,    KC_GRAVE,               TD1,            TD2,                    KC_LBRC,    KC_RBRC,    KC_UP,      KC_DOWN,    XXXXXXX,
                                                        KC_DEL,     KC_BSPC,    XXXXXXX,        TO(QWRT),   KC_ENT,     KC_SPC
    ),
    [AROW] = LAYOUT_moonlander(
        _______,    _______,    _______,    _______,    _______,    _______,    _______,        _______,    _______,    _______,    _______,    _______,    _______,    _______,
        _______,    _______,    _______,    _______,    _______,    _______,    _______,        _______,    _______,    KC_PGDN,    KC_PGUP,    _______,    _______,    _______,
        _______,    _______,    _______,    _______,    _______,    _______,    _______,        _______,    KC_LEFT,    KC_DOWN,    KC_UP,      KC_RIGHT,   _______,    _______,
        _______,    _______,    _______,    _______,    _______,    _______,                                _______,    TO(CLMK),   _______,    _______,    _______,    _______,
        _______,    _______,    _______,    _______,    _______,                _______,        _______,                _______,    _______,    _______,    _______,    _______,
                                                        KC_LALT,    _______,    _______,        TO(CLMK),   _______,    _______
    ),
    [MDIA] = LAYOUT_moonlander(
        _______,    _______,    _______,    _______,    _______,    _______,    _______,        _______,    _______,    _______,    _______,    _______,    _______,    RESET,
        _______,    _______,    _______,    S_BRT_DN,   S_BRT_UP,   _______,    _______,        _______,    M_MUTE,     M_VOL_DN,   M_VOL_UP,   _______,    _______,    _______,
        _______,    _______,    _______,    _______,    _______,    _______,    _______,        _______,    _______,    M_NEXT,     M_PREV,     _______,    _______,    _______,
        _______,    _______,    _______,    _______,    _______,    _______,                                _______,    M_PLAY,     M_STOP,     _______,    _______,    _______,
        _______,    _______,    _______,    _______,    _______,                _______,        _______,                _______,    _______,    _______,    _______,    _______,
                                                        _______,    _______,    _______,        TO(CLMK),   _______,    _______
    ),
    [QWRT] = LAYOUT_moonlander(
        KC_EQUAL,   KC_1,       KC_2,       KC_3,       KC_4,       KC_5,       KC_LEFT,        KC_RIGHT,   KC_6,       KC_7,       KC_8,       KC_9,       KC_0,       KC_MINUS,
        CW_TOGG,    KC_Q,       KC_W,       KC_E,       KC_R,       KC_T,       MO(FUNC),       KC_CAPS,    KC_Y,       KC_U,       KC_I,       KC_O,       KC_P,       KC_BSLS,
        KC_TAB,     QWRT_A,     QWRT_S,     QWRT_D,     QWRT_F,     KC_G,       KC_LBRC,        KC_RBRC,    KC_H,       QWRT_J,     QWRT_K,     QWRT_L,     QWRT_SEMI,  KC_QUOTE,
        KC_LSFT,    KC_Z,       KC_X,       KC_C,       KC_V,       KC_B,                                   KC_N,       KC_M,       KC_COMMA,   KC_DOT,     KC_SLASH,   KC_RSFT,
        KC_LALT,    KC_LEFT,    KC_RIGHT,   KC_LGUI,    KC_GRAVE,               XXXXXXX,        KC_ESC,                 KC_DOWN,    KC_UP,      KC_LEFT,    KC_RIGHT,   XXXXXXX,
                                                        KC_SPACE,   KC_BSPC,    TO(GAME),       TO(CLMK),   KC_ENTER,   KC_SPACE
    ),
    [GAME] = LAYOUT_moonlander(
        KC_6,       KC_1,       KC_2,       KC_3,       KC_4,       KC_5,       KC_ESC,         XXXXXXX,    KC_6,       KC_7,       KC_8,       KC_9,       KC_0,       KC_MINUS,
        KC_CAPS,    KC_Q,       KC_W,       KC_E,       KC_R,       KC_T,       MO(FUNC),       KC_LBRC,    KC_Y,       KC_U,       KC_I,       KC_O,       KC_P,       KC_RBRC,
        KC_TAB,     KC_A,       KC_S,       KC_D,       KC_F,       KC_G,       KC_HYPR,        KC_MEH,     KC_H,       KC_J,       KC_K,       KC_L,       KC_SCLN,    KC_QUOTE,
        KC_LSFT,    KC_Z,       KC_X,       KC_C,       KC_V,       KC_B,                                   KC_N,       KC_M,       KC_COMM,    KC_DOT,     KC_UP,      KC_SLASH,
        KC_LCTL,    KC_GRAVE,   KC_O,       KC_LALT,    MO(GMNM),               KC_ESC,         LSG(KC_R),              KC_LCTL,    KC_PPLS,    KC_LEFT,    KC_DOWN,    KC_RIGHT,
                                                        KC_SPACE,   KC_BSPC,    TO(QWRT),       TO(CLMK),   KC_ENTER,   KC_SPACE
    ),
    [GMNM] = LAYOUT_moonlander(
        XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,        XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,
        XXXXXXX,    KC_9,       KC_8,       KC_7,       XXXXXXX,    XXXXXXX,    XXXXXXX,        XXXXXXX,    XXXXXXX,    KC_7,       KC_8,       KC_9,       XXXXXXX,    XXXXXXX,
        XXXXXXX,    KC_6,       KC_5,       KC_4,       XXXXXXX,    XXXXXXX,    XXXXXXX,        XXXXXXX,    XXXXXXX,    KC_4,       KC_5,       KC_6,       XXXXXXX,    XXXXXXX,
        KC_LSFT,    KC_3,       KC_2,       KC_1,       XXXXXXX,    XXXXXXX,                                XXXXXXX,    KC_1,       KC_2,       KC_3,       XXXXXXX,    XXXXXXX,
        XXXXXXX,    XXXXXXX,    XXXXXXX,    KC_0,       XXXXXXX,                XXXXXXX,        XXXXXXX,                KC_0,       XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,
                                                        XXXXXXX,    XXXXXXX,    XXXXXXX,        TO(GAME),   XXXXXXX,    XXXXXXX
    ),
    [SYMB] = LAYOUT_moonlander(
        _______,    _______,    _______,    _______,    _______,    _______,    _______,        _______,    _______,    KC_CAPS,    KC_SCRL,    KC_NUM,     _______,    _______,
        _______,    _______,    _______,    S(KC_PGDN), S(KC_PGUP), _______,    _______,        _______,    _______,    KC_7,       KC_8,       KC_9,       _______,    _______,
        _______,    KC_PSCR,    _______,    KC_END,     KC_HOME,    _______,    _______,        _______,    _______,    KC_4,       KC_5,       KC_6,       _______,    _______,
        _______,    _______,    _______,    _______,    _______,    _______,                                _______,    KC_1,       KC_2,       KC_3,       _______,    _______,
        _______,    _______,    _______,    _______,    _______,                _______,        _______,                KC_0,       _______,    _______,    _______,    _______,
                                                        MO(FUNC),   _______,    _______,        TO(CLMK),   _______,    _______
    ),
    [FUNC] = LAYOUT_moonlander(
        _______,    _______,    _______,    _______,    _______,    _______,    _______,        _______,    KC_F13,     KC_F14,     KC_F15,     KC_F16,     KC_F20,     KC_F24,
        _______,    _______,    _______,    _______,    _______,    _______,    _______,        _______,    KC_F12,     KC_F7,      KC_F8,      KC_F9,      KC_F19,     KC_F23,
        _______,    KC_LGUI,    KC_LALT,    KC_LSFT,    KC_LCTL,    _______,    _______,        _______,    KC_F11,     KC_F4,      KC_F5,      KC_F6,      KC_F18,     KC_F22,
        _______,    _______,    _______,    _______,    _______,    _______,                                KC_F10,     KC_F1,      KC_F2,      KC_F3,      KC_F17,     KC_F21,
        _______,    _______,    _______,    _______,    _______,                _______,        _______,                _______,    _______,    _______,    _______,    _______,
                                                        _______,    _______,    _______,        TO(CLMK),   _______,    KC_0
    ),
    // [LAYER] = LAYOUT_moonlander(
    //     _______,    _______,    _______,    _______,    _______,    _______,    _______,        _______,    _______,    _______,    _______,    _______,    _______,    _______,
    //     _______,    _______,    _______,    _______,    _______,    _______,    _______,        _______,    _______,    _______,    _______,    _______,    _______,    _______,
    //     _______,    _______,    _______,    _______,    _______,    _______,    _______,        _______,    _______,    _______,    _______,    _______,    _______,    _______,
    //     _______,    _______,    _______,    _______,    _______,    _______,                                _______,    _______,    _______,    _______,    _______,    _______,
    //     _______,    _______,    _______,    _______,    _______,                _______,        _______,                _______,    _______,    _______,    _______,    _______,
    //                                                     _______,    _______,    _______,        _______,    _______,    _______
    // ),
};

const ledmap PROGMEM ledmaps[] = {
    [CLMK] = RGB_LAYOUT(
        ANIMTE, ANIMTE, ANIMTE, ANIMTE, ANIMTE, ANIMTE, PURPLE,     PURPLE, ANIMTE, ANIMTE, ANIMTE, ANIMTE, ANIMTE, ANIMTE,
        GREEN,  ANIMTE, ANIMTE, ANIMTE, ANIMTE, ANIMTE, BLUE,       GREEN,  ANIMTE, ANIMTE, ANIMTE, ANIMTE, ANIMTE, ANIMTE,
        ANIMTE, CYAN,   CYAN,   CYAN,   CYAN,   ANIMTE, CYAN,       CYAN,   ANIMTE, CYAN,   CYAN,   CYAN,   CYAN,   ANIMTE,
        ANIMTE, ANIMTE, ANIMTE, ANIMTE, ANIMTE, ANIMTE,                     ANIMTE, ANIMTE, ANIMTE, ANIMTE, ANIMTE, ANIMTE,
        ANIMTE, PURPLE, PURPLE, ANIMTE, ANIMTE,         ANIMTE,     ANIMTE,         ANIMTE, ANIMTE, PURPLE, PURPLE, ANIMTE,
                                        ANIMTE, ANIMTE, BLUE,       BLUE,   ANIMTE, ANIMTE
    ),
    [AROW] = RGB_LAYOUT(
        ANIMTE, ANIMTE, ANIMTE, ANIMTE, ANIMTE, ANIMTE, ANIMTE,     ANIMTE, ANIMTE, ANIMTE, ANIMTE, ANIMTE, ANIMTE, ANIMTE,
        ANIMTE, ANIMTE, ANIMTE, ANIMTE, ANIMTE, ANIMTE, ANIMTE,     ANIMTE, ANIMTE, BLUE,   BLUE,   ANIMTE, ANIMTE, ANIMTE,
        ANIMTE, ANIMTE, ANIMTE, ANIMTE, ANIMTE, ANIMTE, ANIMTE,     ANIMTE, CYAN,   CYAN,   CYAN,   CYAN,   ANIMTE, ANIMTE,
        ANIMTE, ANIMTE, ANIMTE, ANIMTE, ANIMTE, ANIMTE,                     ANIMTE, GREEN,  ANIMTE, ANIMTE, ANIMTE, ANIMTE,
        ANIMTE, ANIMTE, ANIMTE, ANIMTE, ANIMTE,         ANIMTE,     ANIMTE,         ANIMTE, ANIMTE, ANIMTE, ANIMTE, ANIMTE,
                                        BLUE,   ANIMTE, ANIMTE,     GREEN,  ANIMTE, ANIMTE
    ),
    [MDIA] = RGB_LAYOUT(
        BLUE,   BLUE,   BLUE,   BLUE,   BLUE,   BLUE,   BLUE,       BLUE,   BLUE,   BLUE,   BLUE,   BLUE,   BLUE,   RED,
        BLUE,   BLUE,   BLUE,   YELLOW, YELLOW, BLUE,   BLUE,       BLUE,   RED,    RED,    RED,    BLUE,   BLUE,   BLUE,
        BLUE,   BLUE,   BLUE,   BLUE,   BLUE,   BLUE,   BLUE,       BLUE,   BLUE,   RED,    RED,    BLUE,   BLUE,   BLUE,
        BLUE,   BLUE,   BLUE,   BLUE,   BLUE,   BLUE,                       BLUE,   RED,    RED,    BLUE,   BLUE,   BLUE,
        BLUE,   BLUE,   BLUE,   BLUE,   BLUE,           BLUE,       BLUE,           BLUE,   BLUE,   BLUE,   BLUE,   BLUE,
                                        BLUE,   BLUE,   BLUE,       ______, BLUE,   BLUE
    ),
    [QWRT] = RGB_LAYOUT(
        TEAL,   TEAL,   TEAL,   TEAL,   TEAL,   TEAL,   PURPLE,     PURPLE, TEAL,   TEAL,   TEAL,   TEAL,   TEAL,   TEAL,
        SPRING, TEAL,   TEAL,   TEAL,   TEAL,   TEAL,   GREEN,      SPRING, TEAL,   TEAL,   TEAL,   TEAL,   TEAL,   TEAL,
        TEAL,   RED,    RED,    RED,    RED,    TEAL,   RED,        RED,    TEAL,   RED,    RED,    RED,    RED,    TEAL,
        TEAL,   TEAL,   TEAL,   TEAL,   TEAL,   TEAL,                       TEAL,   TEAL,   TEAL,   TEAL,   TEAL,   TEAL,
        TEAL,   PURPLE, PURPLE, TEAL,   TEAL,           TEAL,       TEAL,           TEAL,   TEAL,   PURPLE, PURPLE, TEAL,
                                        TEAL,   TEAL,   GREEN,      GREEN,  TEAL,   TEAL
    ),
    [GAME] = RGB_LAYOUT(
        RED,    RED,    RED,    RED,    RED,    RED,    GREEN,      XXXXXX, RED,    RED,    RED,    RED,    RED,    RED,
        GREEN,  RED,    CYAN,   RED,    RED,    RED,    BLUE,       RED,    RED,    RED,    RED,    RED,    RED,    RED,
        GREEN,  CYAN,   CYAN,   CYAN,   RED,    RED,    PURPLE,     PURPLE, RED,    RED,    RED,    RED,    RED,    RED,
        PURPLE, CYAN,   CYAN,   CYAN,   RED,    RED,                        RED,    RED,    RED,    RED,    CYAN,   RED,
        PURPLE, RED,    RED,    PURPLE, BLUE,           GREEN,      TURQ,           PURPLE, RED,    CYAN,   CYAN,   CYAN,
                                        GREEN,  GREEN,  BLUE,       BLUE,   GREEN,  GREEN
    ),
    [GMNM] = RGB_LAYOUT(
        RED,    RED,    RED,    RED,    RED,    RED,    RED,        RED,    RED,    RED,    RED,    RED,    RED,    RED,
        RED,    BLUE,   BLUE,   BLUE,   RED,    RED,    RED,        RED,    RED,    BLUE,   BLUE,   BLUE,   RED,    RED,
        RED,    BLUE,   BLUE,   BLUE,   RED,    RED,    RED,        RED,    RED,    BLUE,   BLUE,   BLUE,   RED,    RED,
        PURPLE, BLUE,   BLUE,   BLUE,   RED,    RED,                        RED,    BLUE,   BLUE,   BLUE,   RED,    RED,
        RED,    RED,    RED,    BLUE,   RED,            RED,        RED,            BLUE,   RED,    RED,    RED,    RED,
                                        RED,    RED,    RED,        GREEN,  RED,    RED
    ),
    [SYMB] = RGB_LAYOUT(
        ANIMTE, ANIMTE, ANIMTE, ANIMTE, ANIMTE, ANIMTE, ANIMTE,     ANIMTE, ANIMTE, CYAN,   CYAN,   CYAN,   ANIMTE, ANIMTE,
        ANIMTE, ANIMTE, ANIMTE, ANIMTE, CYAN,   CYAN,   ANIMTE,     ANIMTE, ANIMTE, CYAN,   CYAN,   CYAN,   ANIMTE, ANIMTE,
        ANIMTE, CYAN,   ANIMTE, ANIMTE, CYAN,   CYAN,   ANIMTE,     ANIMTE, ANIMTE, CYAN,   CYAN,   CYAN,   ANIMTE, ANIMTE,
        ANIMTE, ANIMTE, ANIMTE, ANIMTE, ANIMTE, ANIMTE,                     ANIMTE, CYAN,   CYAN,   CYAN,   ANIMTE, ANIMTE,
        ANIMTE, ANIMTE, ANIMTE, ANIMTE, ANIMTE,         ANIMTE,     ANIMTE,         CYAN,   ANIMTE, ANIMTE, ANIMTE, ANIMTE,
                                        GREEN,  ANIMTE, ANIMTE,     GREEN,  ANIMTE, ANIMTE
    ),
    [FUNC] = RGB_LAYOUT(
        ANIMTE, ANIMTE, ANIMTE, ANIMTE, ANIMTE, ANIMTE, ANIMTE,     ANIMTE, PURPLE, PURPLE, PURPLE, PURPLE, PURPLE, PURPLE,
        ANIMTE, ANIMTE, ANIMTE, ANIMTE, ANIMTE, ANIMTE, ANIMTE,     ANIMTE, PURPLE, CYAN,   CYAN,   CYAN,   PURPLE, PURPLE,
        ANIMTE, BLUE,   BLUE,   BLUE,   BLUE,   ANIMTE, ANIMTE,     ANIMTE, PURPLE, CYAN,   CYAN,   CYAN,   PURPLE, PURPLE,
        ANIMTE, ANIMTE, ANIMTE, ANIMTE, ANIMTE, ANIMTE,                     PURPLE, CYAN,   CYAN,   CYAN,   PURPLE, PURPLE,
        ANIMTE, ANIMTE, ANIMTE, ANIMTE, ANIMTE,         ANIMTE,     ANIMTE,         ANIMTE, ANIMTE, ANIMTE, ANIMTE, ANIMTE,
                                        ANIMTE, ANIMTE, ANIMTE,     GREEN,  ANIMTE, CYAN
    ),
    // [LAYER] = RGB_LAYOUT(
    //     ______, ______, ______, ______, ______, ______, ______,     ______, ______, ______, ______, ______, ______, ______,
    //     ______, ______, ______, ______, ______, ______, ______,     ______, ______, ______, ______, ______, ______, ______,
    //     ______, ______, ______, ______, ______, ______, ______,     ______, ______, ______, ______, ______, ______, ______,
    //     ______, ______, ______, ______, ______, ______,                     ______, ______, ______, ______, ______, ______,
    //     ______, ______, ______, ______, ______,         ______,     ______,         ______, ______, ______, ______, ______,
    //                                     ______, ______, ______,     ______, ______, ______
    // ),
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        switch (keycode) {
        case VRSN:
            SEND_STRING (QMK_KEYBOARD "/" QMK_KEYMAP " @ " QMK_VERSION);
            return false;
        }
    }
    return true;
}
