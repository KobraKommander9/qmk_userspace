#pragma once

#include "hrm.h"

void persist_read(void);
void persist_write(void);
void persist_reset(void);

hrm_mode_t persist_get_hrm_mode(void);
hrm_mode_t persist_get_hrm_last_mode(void);
void       persist_set_hrm_mode(hrm_mode_t mode);
void       persist_set_hrm_last_mode(hrm_mode_t mode);
