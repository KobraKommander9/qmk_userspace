#pragma once

#include "keys.h"
#include "quantum.h"

enum {
    TDE_HRM_A,
    TDE_HRM_R,
    TDE_HRM_S,
    TDE_HRM_T,

    TDE_HRM_N,
    TDE_HRM_E,
    TDE_HRM_I,
    TDE_HRM_O,

    TDE_CK_BSPC, // Backspace on tap, delete on Shift+Tap, navigation layer on hold
    TDE_CK_SPC, // Space on tap, numbers layer on hold
    TDE_CK_TAB, // Tab on tap, symbols layer on hold

    TDE_COUNT,
};

bool process_tap_dance_key(uint16_t, keyrecord_t*);
