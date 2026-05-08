#include "sm_td.h"
#include "keys.h"
#include "layers.h"

smtd_resolution on_smtd_action(uint16_t keycode, smtd_action action, uint8_t tap_count) {
    switch (keycode) {
        case CKC_SPC: SMTD_LT(KC_SPC, _NUM); break;
    }

    return SMTD_RESOLUTION_UNHANDLED;
}
