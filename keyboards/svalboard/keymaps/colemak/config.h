#pragma once

#define VIAL_KEYBOARD_UID {0x1B, 0x18, 0x7D, 0xF2, 0x21, 0xF6, 0x29, 0x48}

#define VIAL_UNLOCK_COUNTER_MAX 12

// Indices into dpi_choices[]: {200,400,600,800,1200,...}
#define MOUSE_DPI_400  1
#define MOUSE_DPI_1200 4
#define MOUSE_DPI_2400 6
// Index into mh_timer_choices[]: {200,300,400,500,800,-1}; -1 = no timeout
#define MOUSE_LAYER_TIMEOUT_NONE 2

#ifndef CAPS_WORD_ENABLE
#define CAPS_WORD_ENABLE
#endif

#undef TAPPING_TERM
#define TAPPING_TERM 240

#ifndef TAPPING_TERM_PER_KEY
#define TAPPING_TERM_PER_KEY
#endif

#define USER_TAP_DANCE_ENABLE

#undef PERMISSIVE_HOLD
