#include "tap_dance.h"

#include "hrm.h"
#include "keys.h"

#define OVERLOAD3(_1, _2, _3, NAME, ...) NAME

#define TD_KC(kc)               \
    ((td_action_t){             \
        .type = TD_ACT_KC,      \
        .val.keycode = (kc),    \
    })

#define TD_LON(layer)           \
    ((td_action_t){             \
        .type = TD_ACT_LON,     \
        .val.layer = (layer),   \
    })

#define TD_LOFF(layer)          \
    ((td_action_t){             \
        .type = TD_ACT_LOFF,    \
        .val.layer = (layer),   \
    })

#define TD_LTOGG(layer)         \
    ((td_action_t){             \
        .type = TD_ACT_LTOGG,   \
        .val.layer = (layer),   \
    })

#define TD_FN(...) OVERLOAD3(__VA_ARGS__, TD_FN3, TD_FN2, TD_FN1)(__VA_ARGS__)
#define TD_FN1(fn) TD_FN3(fn, NULL, NULL)
#define TD_FN2(fn, ctxptr) TD_FN3(fn, ctxptr, NULL)
#define TD_FN3(fn, ctxptr, reset)   \
    ((td_action_t){                 \
        .type = TD_ACT_FN,          \
        .val.fn = (fn),             \
        .reset_fn = (reset),        \
        .ctx = (ctxptr),            \
    })

#define TD_MAP(s, a) \
    { .state = (s), .action = (a) }

#define TD_CFG(name, ...)                           \
    static const td_action_map_t name##_map[] = {   \
        __VA_ARGS__                                 \
    };                                              \
    static const td_config_t name##_cfg = {         \
        .map = name##_map,                          \
        .len = ARRAY_SIZE(name##_map),              \
    }

#define TD_RUNTIME(name, cfgname)               \
    static td_runtime_t name##_rt = {           \
        .cfg = &cfgname##_cfg,                  \
        .resolve = NULL,                        \
        .resolve_ctx = NULL,                    \
        .cfg_bound = true,                      \
        .committed = false,                     \
    }
#define TD_RUNTIME_RESOLVED(name, resolver, ctxptr) \
    static td_runtime_t name##_rt = {               \
        .cfg = NULL,                                \
        .resolve = (resolver),                      \
        .resolve_ctx = (ctxptr),                    \
        .cfg_bound = false,                         \
        .committed = false,                         \
    }

#define TD_ACTION(name)                                                         \
    {.fn = {NULL, on_dance_finished, on_dance_reset, on_dance_release },        \
        .user_data = (void *)&name##_rt }

// ----------------------------------------------------------------------------

static inline void td_bind_cfg(td_runtime_t *runtime) {
    if (runtime->cfg_bound || runtime->resolve == NULL)
        return;

    runtime->cfg = runtime->resolve(runtime->resolve_ctx);
    runtime->cfg_bound = (runtime->cfg != NULL);
}

static td_depth_t compute_depth(const td_config_t *cfg) {
    td_depth_t depth = TD_DEPTH_SINGLE;

    for (uint8_t i = 0; i < cfg->len; ++i) {
        td_state_t state = cfg->map[i].state;
        switch (state) {
            case TD_DOUBLE_TAP:
            case TD_DOUBLE_HOLD:
            case TD_DOUBLE_SINGLE_TAP:
                if (depth < TD_DEPTH_DOUBLE)
                    depth = TD_DEPTH_DOUBLE;
                break;

            case TD_TRIPLE_TAP:
            case TD_TRIPLE_HOLD:
            case TD_TRIPLE_SINGLE_TAP:
                return TD_DEPTH_TRIPLE;

            default:
                break;
        }
    }

    return depth;
}

static bool can_commit_early(td_state_t state, const td_config_t *cfg) {
    switch (state) {
        case TD_SINGLE_TAP:
        case TD_SINGLE_HOLD:
            return compute_depth(cfg) == TD_DEPTH_SINGLE;

        case TD_DOUBLE_TAP:
        case TD_DOUBLE_SINGLE_TAP:
            return compute_depth(cfg) <= TD_DEPTH_DOUBLE;

        case TD_TRIPLE_TAP:
        case TD_TRIPLE_SINGLE_TAP:
            return true;

        default:
            return false;
    }
}

static inline td_action_t td_action_none(void) {
    return (td_action_t){ .type = TD_ACT_NONE };
}

static td_action_t lookup_action(const td_runtime_t *runtime, td_state_t state) {
    if (!runtime->cfg)
        return td_action_none();

    const td_config_t *cfg = runtime->cfg;

    for (uint8_t i = 0; i < cfg->len; ++i) {
        if (cfg->map[i].state == state) {
            return cfg->map[i].action;
        }
    }

    return td_action_none();
}

static void execute_action(td_runtime_t *runtime, td_action_t action) {
    if (runtime->committed || action.type == TD_ACT_NONE)
        return;

    runtime->active.action = action;
    runtime->active.active = true;
    runtime->committed = true;

    switch (action.type) {
        case TD_ACT_KC:
            register_code16(action.val.keycode);
            break;

        case TD_ACT_LON:
            layer_on(action.val.layer);
            break;

        case TD_ACT_LOFF:
            layer_off(action.val.layer);
            break;

        case TD_ACT_LTOGG:
            layer_invert(action.val.layer);
            break;

        case TD_ACT_FN:
            if (action.val.fn) action.val.fn(action.ctx);
            break;

        default:
            break;
    }
}

static td_state_t dance_step(tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed) return TD_SINGLE_TAP;
        return TD_SINGLE_HOLD;
    }

    if (state->count == 2) {
        if (state->interrupted) return TD_DOUBLE_SINGLE_TAP;
        else if (state->pressed) return TD_DOUBLE_HOLD;
        return TD_DOUBLE_TAP;
    }

    if (state->count == 3) {
        if (state->interrupted) return TD_TRIPLE_SINGLE_TAP;
        else if (state->pressed) return TD_TRIPLE_HOLD;
        return TD_TRIPLE_TAP;
    }

    return TD_UNKNOWN;
}

static void on_dance_finished(tap_dance_state_t *state, void *user_data) {
    td_runtime_t *runtime = user_data;

    td_bind_cfg(runtime);
    if (!runtime->cfg)
        return;

    td_state_t td_state = dance_step(state);
    td_action_t action = lookup_action(runtime, td_state);

    execute_action(runtime, action);
}

static void on_dance_release(tap_dance_state_t *state, void *user_data) {
    td_runtime_t *runtime = user_data;

    td_bind_cfg(runtime);
    if (!runtime->cfg || runtime->committed)
        return;

    td_state_t td_state = dance_step(state);
    if (can_commit_early(td_state, runtime->cfg)) {
        td_action_t action = lookup_action(runtime, td_state);
        execute_action(runtime, action);
        if (runtime->committed)
            state->finished = true;
    }
}

static void on_dance_reset(tap_dance_state_t *state, void *user_data) {
    td_runtime_t *runtime = user_data;
    (void)state;

    if (runtime->active.active) {
        td_action_t *action = &runtime->active.action;

        switch (action->type) {
            case TD_ACT_KC:
                unregister_code16(action->val.keycode);
                break;

            default:
                if (action->reset_fn)
                    action->reset_fn(action->ctx);
                break;
        }
    }


    runtime->active.active = false;
    runtime->committed = false;

    if (runtime->resolve) {
        runtime->cfg_bound = false;
        runtime->cfg = NULL;
    }
}

// ----------------------------------------------------------------------------
// HRM

typedef struct {
    hrm_hand_t hand;

    const td_config_t *enabled;
    const td_config_t *disabled;
} hrm_td_ctx_t;

static const td_config_t *hrm_resolver(const void *ctx) {
    const hrm_td_ctx_t *hrm = ctx;
    return hrm_active(hrm->hand)
        ? hrm->enabled
        : hrm->disabled;
}

#define TD_HRM(name, kc, mod, hrm_hand)                 \
    TD_CFG(name##_enabled,                              \
        TD_MAP(TD_SINGLE_TAP, TD_KC(kc)),               \
        TD_MAP(TD_SINGLE_HOLD, TD_KC(mod))              \
    );                                                  \
    TD_CFG(name##_disabled,                             \
        TD_MAP(TD_SINGLE_TAP, TD_KC(kc))                \
    );                                                  \
    static const hrm_td_ctx_t name##_ctx = {            \
        .hand = (hrm_hand),                             \
        .enabled = &name##_enabled_cfg,                 \
        .disabled = &name##_disabled_cfg,               \
    };                                                  \
    TD_RUNTIME_RESOLVED(name, hrm_resolver, &name##_ctx)

TD_HRM(hrm_a, KC_A, KC_LGUI, HRM_HAND_LEFT);
TD_HRM(hrm_r, KC_R, KC_LALT, HRM_HAND_LEFT);
TD_HRM(hrm_s, KC_S, KC_LSFT, HRM_HAND_LEFT);
TD_HRM(hrm_t, KC_T, KC_LCTL, HRM_HAND_LEFT);

TD_HRM(hrm_n, KC_N, KC_RCTL, HRM_HAND_RIGHT);
TD_HRM(hrm_e, KC_E, KC_RSFT, HRM_HAND_RIGHT);
TD_HRM(hrm_i, KC_I, KC_LALT, HRM_HAND_RIGHT);
TD_HRM(hrm_o, KC_O, KC_RGUI, HRM_HAND_RIGHT);

// ----------------------------------------------------------------------------

USER_TAP_DANCE_TABLE(
    [TDE_HRM_A] = TD_ACTION(hrm_a),
    [TDE_HRM_R] = TD_ACTION(hrm_r),
    [TDE_HRM_S] = TD_ACTION(hrm_s),
    [TDE_HRM_T] = TD_ACTION(hrm_t),

    [TDE_HRM_N] = TD_ACTION(hrm_n),
    [TDE_HRM_E] = TD_ACTION(hrm_e),
    [TDE_HRM_I] = TD_ACTION(hrm_i),
    [TDE_HRM_O] = TD_ACTION(hrm_o),
);
