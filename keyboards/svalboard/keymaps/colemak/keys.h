#pragma once

#include "keymap_support.h"

#define MKC_BKTAB S(KC_TAB)         // Back Tab
#define MKC_APPWIN C(G(KC_DOWN))    // Show windows from current app
#define MKC_DKTN C(G(KC_RGHT))      // Next Desktop
#define MKC_DKTP C(G(KC_LEFT))      // Previous Desktop
#define MKC_SHDKT A(G(KC_DOWN))     // Show Desktop
#define MKC_SHNOT A(G(KC_UP))       // Show Notification Center
#define MKC_SPFND A(G(KC_RIGHT))    // Spotlight Find

#define SV_LDPU SV_LEFT_DPI_INC
#define SV_LDPD SV_LEFT_DPI_DEC
#define SV_RDPU SV_RIGHT_DPI_INC
#define SV_RDPD SV_RIGHT_DPI_DEC
#define SV_SOUT SV_OUTPUT_STATUS

#define RANGE_START QK_KB_20
enum custom_keycodes {
    // Home Row Mods
    HRM_A = RANGE_START,
    HRM_R,
    HRM_S,
    HRM_T,

    HRM_N,
    HRM_E,
    HRM_I,
    HRM_O,

    HRM_TOGGLE,
    HRM_CYCLE,
    HRM_TD, // Tap for Toggle, Double Tap for Cycle

    // Tap Dances
    CKC_BSPC, // Backspace on tap, delete on Shift+Tap, navigation layer on hold
    CKC_SPC, // Space on tap, numbers layer on hold
    CKC_TAB, // Tab on tap, symbols layer on hold
};

extern void mouse_mode(bool on);
