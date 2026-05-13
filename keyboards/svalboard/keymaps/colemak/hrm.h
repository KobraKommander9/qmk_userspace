#pragma once

#include "quantum.h"

typedef enum {
    HRM_MODE_OFF,
    HRM_MODE_LEFT,
    HRM_MODE_RIGHT,
    HRM_MODE_ON,
    HRM_MODE_COUNT,
} hrm_mode_t;

typedef enum {
    HRM_HAND_LEFT,
    HRM_HAND_RIGHT,
} hrm_hand_t;

uint16_t get_hrm_tapping_term(uint16_t, keyrecord_t*);
bool hrm_active(hrm_hand_t);
void init_hrm_eeprom(void);
bool is_hrm_keycode(uint16_t);
bool process_hrm(uint16_t, keyrecord_t*);
void setup_hrm_keys(void);
