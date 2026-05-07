#include QMK_KEYBOARD_H
#include "combos.h"

const uint16_t PROGMEM zc_combo[] = {KC_Z, KC_C, COMBO_END};
const uint16_t PROGMEM zv_combo[] = {KC_Z, KC_V, COMBO_END};
const uint16_t PROGMEM zxc_combo[] = {KC_Z, KC_X, KC_C, COMBO_END};
const uint16_t PROGMEM zxv_combo[] = {KC_Z, KC_X, KC_V, COMBO_END};

combo_t key_combos[] = {
    [ZC_COPY] = COMBO(zc_combo, LCTL(KC_C)),
    [ZV_PASTE] = COMBO(zv_combo, LCTL(KC_V)),
    [ZXC_COPY] = COMBO(zxc_combo, LCTL(LSFT(KC_C))),
    [ZXV_PASTE] = COMBO(zxv_combo, LCTL(LSFT(KC_V))),
};
