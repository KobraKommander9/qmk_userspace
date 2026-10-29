#include <string.h>

#include "layers.h"
#include "persist.h"
#include "svalboard.h"

static const struct layer_hsv layer_colors[] = {
    [_BASE]  = {HSV_GREEN},
    [_GAME1] = {HSV_BLUE},
    [_GAME2] = {HSV_AZURE},
    [_SYM]   = {HSV_MAGENTA},
    [_NUM]   = {HSV_YELLOW},
    [_NAV]   = {HSV_CYAN},
    [_MEDIA] = {HSV_PURPLE},
    [_SYS]   = {HSV_RED},
    [_MBO]   = {HSV_PINK},
};

void setup_rgb_colors(void) {
    if (memcmp(global_saved_values.layer_colors, layer_colors, sizeof(layer_colors)) == 0) return;

    memcpy(global_saved_values.layer_colors, layer_colors, sizeof(layer_colors));
    write_eeprom_kb();
}
