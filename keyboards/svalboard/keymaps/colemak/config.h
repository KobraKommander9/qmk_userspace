#pragma once

// Indices into dpi_choices[]: {200,400,600,800,1200,...}
#define MOUSE_DPI_400   1
#define MOUSE_DPI_600   2
#define MOUSE_DPI_800   3
#define MOUSE_DPI_1200  4
#define MOUSE_DPI_2400  6
// Index into mh_timer_choices[]: {200,300,400,500,800,-1}; -1 = no timeout
#define MOUSE_LAYER_TIMEOUT_NONE 2

#ifndef CAPS_WORD_ENABLE
#define CAPS_WORD_ENABLE
#endif

#undef TAPPING_TERM
#define TAPPING_TERM 200

#ifndef TAPPING_TERM_PER_KEY
#define TAPPING_TERM_PER_KEY
#endif

#undef PERMISSIVE_HOLD
