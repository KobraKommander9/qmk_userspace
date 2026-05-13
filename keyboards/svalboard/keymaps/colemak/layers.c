#include "layers.h"
#include "persist.h"
#include "svalboard.h"

#define HSV(c) (struct layer_hsv) { (c >> 16) & 0xFF, (c >> 8) & 0xFF, c & 0xFF}

void setup_rgb_colors(void) {
    global_saved_values.layer_colors[_BASE] = HSV(0x55FFFF);
    global_saved_values.layer_colors[_GAME1] = HSV(0xAAE0FF);
    global_saved_values.layer_colors[_GAME2] = HSV(0x95E0FF);
    global_saved_values.layer_colors[_NAV]   = HSV(0x80FFFF);
    global_saved_values.layer_colors[_NUM]   = HSV(0x2BFFFF);
    global_saved_values.layer_colors[_SYM]   = HSV(0xD5FFFF);
    for (int i = _SYM + 1; i < _SYS; i++) {
        global_saved_values.layer_colors[i]  = HSV(0x00FFFF);
    }
    global_saved_values.layer_colors[_SYS]   = HSV(0x15FFFF);
    global_saved_values.layer_colors[_MBO]   = HSV(0xEAFFFF);

    write_eeprom_kb();
}
