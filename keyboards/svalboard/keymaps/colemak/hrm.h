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

void init_hrm_eeprom(void);
void setup_hrm_keys(void);
bool hrm_active(hrm_hand_t hand);
bool process_hrm(uint16_t, keyrecord_t*);
