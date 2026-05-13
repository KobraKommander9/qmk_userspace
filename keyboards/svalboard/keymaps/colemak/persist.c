#include "persist.h"
#include "eeprom.h"

typedef union {
    uint32_t raw;
    struct {
        hrm_mode_t hrm_mode        : 3;
        hrm_mode_t hrm_last_mode   : 3;
    };
} user_config_t;

static user_config_t user_config;

void persist_write(void) {
    eeconfig_update_user(user_config.raw);
}

void persist_reset(void) {
    user_config.raw               = 0;
    user_config.hrm_mode          = HRM_MODE_ON;
    user_config.hrm_last_mode     = HRM_MODE_ON;
    persist_write();
}

void persist_read(void) {
    user_config.raw = eeconfig_read_user();
    if (user_config.raw == 0 || user_config.hrm_mode >= HRM_MODE_COUNT) {
        persist_reset();
    }
}

hrm_mode_t persist_get_hrm_mode(void) {
    return user_config.hrm_mode;
}

hrm_mode_t persist_get_hrm_last_mode(void) {
    return user_config.hrm_last_mode;
}

void persist_set_hrm_mode(hrm_mode_t mode) {
    user_config.hrm_mode = mode;
    persist_write();
}

void persist_set_hrm_last_mode(hrm_mode_t mode) {
    user_config.hrm_last_mode = mode;
    persist_write();
}
