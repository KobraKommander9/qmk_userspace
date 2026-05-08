#pragma once

#include "keymap_support.h"

#define MKC_BKTAB S(KC_TAB)         // Back Tab
#define MKC_APPWIN C(G(KC_DOWN))    // Show windows from current app
#define MKC_DKTN C(G(KC_RGHT))      // Next Desktop
#define MKC_DKTP C(G(KC_LEFT))      // Previous Desktop
#define MKC_SHDKT A(G(KC_DOWN))     // Show Desktop
#define MKC_SHNOT A(G(KC_UP))       // Show Notification Center
#define MKC_SPFND A(G(KC_RIGHT))    // Spotlight Find

// Indices into dpi_choices[]: {200,400,600,800,1200,...}
#define MOUSE_DPI_400  1
#define MOUSE_DPI_1200 4
// Index into mh_timer_choices[]: {200,300,400,500,800,-1}; -1 = no timeout
#define MOUSE_LAYER_TIMEOUT_NONE 5

#define SV_LDPU SV_LEFT_DPI_INC
#define SV_LDPD SV_LEFT_DPI_DEC
#define SV_RDPU SV_RIGHT_DPI_INC
#define SV_RDPD SV_RIGHT_DPI_DEC
#define SV_SOUT SV_OUTPUT_STATUS

typedef enum {
    HRM_MODE_OFF,
    HRM_MODE_LEFT,
    HRM_MODE_RIGHT,
    HRM_MODE_ON,
    HRM_MODE_COUNT,
} hrm_mode_t;

typedef enum {
    HRM_HAND_LEFT,
    HRM_HAND_RIGHT,
} hrm_hand_t;

#define RANGE_START QK_KB_20
enum custom_keycodes {
    // left hand Home Row Mods
    HRM_A = RANGE_START,
    HRM_R,
    HRM_S,
    HRM_T,

    // right hand Home Row Mods
    HRM_N,
    HRM_E,
    HRM_I,
    HRM_O,

    // Home Row Mod toggles
    HRM_TOGGLE,
    HRM_CYCLE,
};

void init_hrm_eeprom(void);
void setup_hrm_keys(void);

#define HANDLE_HRM(tp_kc, mod_kc, hand) \
    if (!hrm_active(hand)) { \
        if (record->event.pressed) register_code(tap_kc); \
        else unregister_code(tap_kc); \
        return false; \
    } \
    record->keycode = mod_kc; \
    return true;
