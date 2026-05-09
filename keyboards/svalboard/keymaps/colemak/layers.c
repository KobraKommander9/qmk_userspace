#include "layers.h"
#include "persist.h"
#include "svalboard.h"

void setup_rgb_colors(void) {
    if (persist_colors_initialized()) return;

    global_saved_values.layer_colors[_BASE]  = (struct layer_hsv){  70, 200, 180 };
    global_saved_values.layer_colors[_GAME1] = (struct layer_hsv){ 153, 255, 180 };
    global_saved_values.layer_colors[_GAME2] = (struct layer_hsv){ 137, 255, 180 };
    global_saved_values.layer_colors[_NAV]   = (struct layer_hsv){  20, 255, 180 };
    global_saved_values.layer_colors[_NUM]   = (struct layer_hsv){ 143, 255, 180 };
    global_saved_values.layer_colors[_SYM]   = (struct layer_hsv){  11, 176, 255 };
    for (int i = _SYM + 1; i < _SYS; i++) {
        global_saved_values.layer_colors[i]  = (struct layer_hsv){ 128, 255, 128 };
    }
    global_saved_values.layer_colors[_SYS]   = (struct layer_hsv){   0, 255, 180 };
    global_saved_values.layer_colors[_MBO]   = (struct layer_hsv){ 208, 255, 180 };

    write_eeprom_kb();
    persist_mark_colors_initialized();
}
