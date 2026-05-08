#include "layers.h"
#include "rgblight.h"
#include "color.h"

#define BASE_GREEN      70, 200, 180
#define GAME1_BLUE      153, 255, 180
#define GAME2_BLUE      137, 255, 180
#define NAV_ORANGE      20, 255, 180
#define NUM_BLUE        143, 255, 180
#define SYS_RED         0, 255, 180
#define MBO_MAGENTA     208, 255, 180

extern void mouse_mode(bool on);

#define LAYER_COLOR(name, color) rgblight_segment_t const (name)[] = RGBLIGHT_LAYER_SEGMENTS({0, 2, color})
LAYER_COLOR(layer0_colors, BASE_GREEN);
LAYER_COLOR(layer1_colors, GAME1_BLUE);
LAYER_COLOR(layer2_colors, GAME2_BLUE);
LAYER_COLOR(layer3_colors, NAV_ORANGE);
LAYER_COLOR(layer4_colors, NUM_BLUE);
LAYER_COLOR(layer5_colors, HSV_CORAL);
LAYER_COLOR(layer6_colors, HSV_TEAL);
LAYER_COLOR(layer7_colors, HSV_TEAL);
LAYER_COLOR(layer8_colors, HSV_TEAL);
LAYER_COLOR(layer9_colors, HSV_TEAL);
LAYER_COLOR(layer10_colors, HSV_TEAL);
LAYER_COLOR(layer11_colors, HSV_TEAL);
LAYER_COLOR(layer12_colors, HSV_TEAL);
LAYER_COLOR(layer13_colors, HSV_TEAL);
LAYER_COLOR(layer14_colors, SYS_RED);
LAYER_COLOR(layer15_colors, MBO_MAGENTA);
#undef LAYER_COLOR

const rgblight_segment_t* const rgb_layers[] = RGBLIGHT_LAYERS_LIST(
    layer0_colors, layer1_colors, layer2_colors, layer3_colors,
    layer4_colors, layer5_colors, layer6_colors, layer7_colors,
    layer8_colors, layer9_colors, layer10_colors, layer11_colors,
    layer12_colors, layer13_colors, layer14_colors, layer15_colors
);

layer_state_t set_default_rgb_light(layer_state_t state) {
    rgblight_set_layer_state(0, layer_state_cmp(state, 0));
    return state;
}

layer_state_t set_rgb_light(layer_state_t state) {
    for (int i = 0; i < RGBLIGHT_LAYERS; ++i) {
        rgblight_set_layer_state(i, layer_state_cmp(state, i));
    }

    return state;
}

void setup_rgb_light_layer() {
    rgblight_layers = rgb_layers;
}
