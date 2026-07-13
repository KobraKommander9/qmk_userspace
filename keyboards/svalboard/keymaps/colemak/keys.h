#pragma once

#include "tap_dance.h"
#include "keymap_support.h"

#define SV_LDPU SV_LEFT_DPI_INC
#define SV_LDPD SV_LEFT_DPI_DEC
#define SV_RDPU SV_RIGHT_DPI_INC
#define SV_RDPD SV_RIGHT_DPI_DEC
#define SV_SOUT SV_OUTPUT_STATUS

#define M_MUTE KC_AUDIO_MUTE
#define M_VOL_DN KC_AUDIO_VOL_DOWN
#define M_VOL_UP KC_AUDIO_VOL_UP
#define M_NEXT KC_MEDIA_NEXT_TRACK
#define M_PREV KC_MEDIA_PREV_TRACK
#define M_PLAY KC_MEDIA_PLAY_PAUSE
#define M_STOP KC_MEDIA_STOP

#define HRM_A LGUI_T(KC_A)
#define HRM_R LALT_T(KC_R)
#define HRM_S LSFT_T(KC_S)
#define HRM_T LCTL_T(KC_T)

#define HRM_N RCTL_T(KC_N)
#define HRM_E RSFT_T(KC_E)
#define HRM_I LALT_T(KC_I)
#define HRM_O RGUI_T(KC_O)

#define LT_SPC LT(_SYM,KC_SPC)
#define LT_BSPC LT(_NUM,KC_BSPC)

#define CKC_BSPC USER_TD(TDE_CK_BSPC)
#define CKC_EXLM USER_TD(TDE_CK_EXLM)
#define CKC_GAME USER_TD(TDE_CK_GAME)
#define CKC_GRV USER_TD(TDE_CK_GRV)
#define CKC_HYPR USER_TD(TDE_CK_HYPR)
#define CKC_MEDIA USER_TD(TDE_CK_MEDIA)
#define CKC_MEH USER_TD(TDE_CK_MEH)
#define CKC_SPC USER_TD(TDE_CK_SPC)
#define CKC_TAB USER_TD(TDE_CK_TAB)

#define RANGE_START QK_KB_20
enum custom_keycodes {
    // Home Row Mods
    HRM_TOGGLE = RANGE_START,
    HRM_CYCLE,

    // Macros
    M_ARR,
    M_FAT,
    M_SCP,
    M_WAL,
    M_AND,
    M_OR,
};

extern void mouse_mode(bool on);
