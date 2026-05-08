#pragma once

#include "rgblight.h"
#include "keymap_support.h"

#define LAST_LAYER MH_AUTO_BUTTONS_LAYER

extern const rgblight_segment_t* const rgb_layers[];

enum layer_names {
    _BASE,                  // The default keyboard layer.
    _NUM,                   // The numbers layer.
    _SYS = LAST_LAYER - 1,  // Svalboard specific keys layer.
    _MBO = LAST_LAYER,      // Mouse buttons layer.
};

void setup_rgb_light_layer(void);
layer_state_t set_default_rgb_light(layer_state_t);
layer_state_t set_rgb_light(layer_state_t);
