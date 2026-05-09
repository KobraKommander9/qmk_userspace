#include "sm_td.h"
#include "keys.h"
#include "layers.h"

smtd_resolution on_smtd_action(uint16_t keycode, smtd_action action, uint8_t tap_count) {
    switch (keycode) {
        SMTD_LT(CKC_SPC, KC_SPC, _NUM)
        SMTD_LT(CKC_TAB, KC_TAB, _SYM)
    }

    return SMTD_RESOLUTION_UNHANDLED;
}
