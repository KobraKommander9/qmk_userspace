#pragma once

#include "keymap_support.h"

#define LAST_LAYER MH_AUTO_BUTTONS_LAYER

enum layer_names {
    _BASE,                  // The default keyboard layer.
    _GAME1,                 // Base game layer.
    _GAME2,                 // Second game layer.
    _NAV,                   // Navigation layer.
    _NUM,                   // Numbers layer.
    _SYM,                   // Symbol layer.
    _SYS = LAST_LAYER - 1,  // Svalboard specific keys layer.
    _MBO = LAST_LAYER,      // Mouse buttons layer.
};

void setup_rgb_colors(void);
