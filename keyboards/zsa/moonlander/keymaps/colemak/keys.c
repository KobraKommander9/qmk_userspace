#include QMK_KEYBOARD_H
#include "keys.h"

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
     switch (keycode) {
         case HOME_R:
         case QWRT_S:
             return TAPPING_TERM - 20;

         case HOME_S:
         case QWRT_D:
             return TAPPING_TERM - 75;

         case HOME_E:
         case QWRT_K:
             return TAPPING_TERM - 75;

         case HOME_I:
         case QWRT_L:
             return TAPPING_TERM - 20;

         default:
             return TAPPING_TERM;
     }
 }
