#include "macros.h"

#include "keys.h"

bool process_macro(uint16_t keycode, keyrecord_t *record) {
    if (!record->event.pressed) {
        return true;
    }

    switch (keycode) {
        case M_AND: SEND_STRING("&&"); break;
        case M_ARR: SEND_STRING("->"); break;
        case M_FAT: SEND_STRING("=>"); break;
        case M_OR: SEND_STRING("||"); break;
        case M_SCP: SEND_STRING("::"); break;
        case M_WAL: SEND_STRING(":="); break;
    }

    return true;
}
