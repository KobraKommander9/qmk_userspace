#pragma once

#include "keymap_support.h"

enum {
    TDE_CK_BSPC, // Backspace on tap, delete on Shift+Tap, navigation layer on hold
    TDE_CK_SPC, // Space on tap, numbers layer on hold
    TDE_CK_TAB, // Tab on tap, symbols layer on hold
    TDE_CK_MEDIA, // media layer on double tap, hrm toggle on triple tap
    TDE_CK_GAME, // game layer on double tap
    TDE_CK_EXLM, // ! on tap, & on shift + tap, * on double tap, # on shift + double tap
    TDE_CK_GRV, // KC_GRV on tap, KC_PERC on double tap

    TDE_COUNT,
};
