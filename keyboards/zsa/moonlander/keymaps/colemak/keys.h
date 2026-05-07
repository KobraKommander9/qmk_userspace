#pragma once

enum custom_keycodes {
    VRSN = SAFE_RANGE,
};

// Left-hand home row mods
#define HOME_A LGUI_T(KC_A)
#define HOME_R LALT_T(KC_R)
#define HOME_S LSFT_T(KC_S)
#define HOME_T LCTL_T(KC_T)

#define QWRT_A LGUI_T(KC_A)
#define QWRT_S LALT_T(KC_S)
#define QWRT_D LSFT_T(KC_D)
#define QWRT_F LCTL_T(KC_F)

// Right-hand home row mods
#define HOME_N RCTL_T(KC_N)
#define HOME_E RSFT_T(KC_E)
#define HOME_I LALT_T(KC_I)
#define HOME_O RGUI_T(KC_O)

#define QWRT_J RCTL_T(KC_J)
#define QWRT_K RSFT_T(KC_K)
#define QWRT_L LALT_T(KC_L)
#define QWRT_SEMI RGUI_T(KC_SCLN)

// Software keys
#define S_BRT_DN KC_BRIGHTNESS_DOWN
#define S_BRT_UP KC_BRIGHTNESS_UP

// Media keys
#define M_MUTE KC_AUDIO_MUTE
#define M_VOL_DN KC_AUDIO_VOL_DOWN
#define M_VOL_UP KC_AUDIO_VOL_UP
#define M_NEXT KC_MEDIA_NEXT_TRACK
#define M_PREV KC_MEDIA_PREV_TRACK
#define M_PLAY KC_MEDIA_PLAY_PAUSE
#define M_STOP KC_MEDIA_STOP

// Custom mods
#define C_REFRESH LGUI(LSFT(KC_R))
