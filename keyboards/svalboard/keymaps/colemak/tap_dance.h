#pragma once

#include "keys.h"

enum {
    TDE_HRM_A,
    TDE_HRM_R,
    TDE_HRM_S,
    TDE_HRM_T,

    TDE_HRM_N,
    TDE_HRM_E,
    TDE_HRM_I,
    TDE_HRM_O,

    TDE_CK_BSPC, // Backspace on tap, delete on Shift+Tap, navigation layer on hold
    TDE_CK_SPC, // Space on tap, numbers layer on hold
    TDE_CK_TAB, // Tab on tap, symbols layer on hold

    TDE_COUNT,
};

typedef enum {
    TD_NONE,
    TD_UNKNOWN,
    TD_SINGLE_TAP,
    TD_SINGLE_HOLD,
    TD_DOUBLE_TAP,
    TD_DOUBLE_HOLD,
    TD_DOUBLE_SINGLE_TAP,
    TD_TRIPLE_TAP,
    TD_TRIPLE_HOLD,
    TD_TRIPLE_SINGLE_TAP,
} td_state_t;

typedef enum {
    TD_DEPTH_SINGLE = 1,
    TD_DEPTH_DOUBLE = 2,
    TD_DEPTH_TRIPLE = 3,
} td_depth_t;

typedef enum {
    TD_ACT_NONE,
    TD_ACT_KC,
    TD_ACT_LON,
    TD_ACT_LOFF,
    TD_ACT_LTOGG,
    TD_ACT_FN,
} td_action_type_t;

typedef void (*td_action_fn_t)(void *ctx);

typedef struct {
    td_action_type_t type;

    union {
        uint16_t keycode;
        uint8_t layer;
        td_action_fn_t fn;
    } val;

    void (*reset_fn)(void *ctx);
    void *ctx;
} td_action_t;

typedef struct {
    td_action_t action;
    td_state_t state;
} td_action_map_t;

typedef struct {
    const td_action_map_t *map;
    uint8_t len;
} td_config_t;

typedef const td_config_t* (*td_cfg_resolver_t)(const void *ctx);

typedef struct {
    td_action_t action;
    bool active;
} td_action_instance_t;

typedef struct {
    const td_config_t *cfg;

    td_cfg_resolver_t resolve;
    const void *resolve_ctx;
    bool cfg_bound;

    td_action_instance_t active;
    bool committed;
} td_runtime_t;
