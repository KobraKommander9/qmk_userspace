#include QMK_KEYBOARD_H
#include "layers.h"

void housekeeping_task_user(void) {
    if (!is_transport_connected()) {
        layer_move(GAME);
    }
}
