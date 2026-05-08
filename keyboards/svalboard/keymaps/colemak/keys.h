#pragma once

#define HOME_A LGUI_T(KC_A)
#define HOME_R LALT_T(KC_R)
#define HOME_S LSFT_T(KC_S)
#define HOME_T LCTL_T(KC_T)

#define HOME_N RCTL_T(KC_N)
#define HOME_E RSFT_T(KC_E)
#define HOME_I LALT_T(KC_I)
#define HOME_O RGUI_T(KC_O)

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
