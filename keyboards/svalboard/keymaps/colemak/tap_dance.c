#include "tap_dance.h"

#include "hrm.h"
#include "layers.h"
#include "keys.h"

// ----------------------------------------------------------------------------
// Types

typedef enum {
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
    TD_DEPTH_SINGLE,
    TD_DEPTH_DOUBLE,
    TD_DEPTH_TRIPLE,
} td_depth_t;

typedef enum {
    TD_ACT_NONE,
    TD_ACT_KC,
    TD_ACT_KC_SFT,
    TD_ACT_MOD,
    TD_ACT_LMV,
    TD_ACT_FN,
} td_action_type_t;

typedef struct {
    td_action_type_t type;

    union {
        uint16_t keycode;
        uint8_t layer;

        struct {
            uint16_t base;
            uint16_t shifted;
        } kc_pair;

        void (*fn)(void *);
    } val;

    void (*reset_fn)(void *);
    void *ctx;
} td_action_t;

typedef struct {
    td_state_t state;
    td_action_t action;
} td_action_map_t;

typedef struct {
    const td_action_map_t *map;
    uint8_t len;
} td_config_t;

typedef const td_config_t *(*td_resolver_t)(const void *);

typedef struct {
    td_action_t action;
    bool active;
    uint16_t emitted_kc;
} td_action_instance_t;

typedef struct {
    const td_config_t *cfg;

    td_resolver_t resolve;
    const void *resolve_ctx;

    td_action_instance_t active;
} td_runtime_t;

typedef enum {
    TD_EXEC_FINISHED,
    TD_EXEC_RELEASE,
} td_exec_origin_t;

// ----------------------------------------------------------------------------
// Action constructors

#define OVERLOAD3(_1, _2, _3, NAME, ...) NAME

#define TD_KC(kc)               \
    ((td_action_t){             \
        .type = TD_ACT_KC,      \
        .val.keycode = (kc),    \
    })

#define TD_KC_SFT(base, shifted)                \
    ((td_action_t){                             \
        .type = TD_ACT_KC_SFT,                  \
        .val.kc_pair = { (base), (shifted) },   \
    })

#define TD_MOD(kc)              \
    ((td_action_t){             \
        .type = TD_ACT_MOD,     \
        .val.keycode = (kc),    \
    })

#define TD_LMV(l)           \
    ((td_action_t){         \
        .type = TD_ACT_LMV, \
        .val.layer = (l),   \
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

#define TD_MAP(state_, action_) \
    { .state = (state_), .action = (action_) }

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
    }

#define TD_RUNTIME_RESOLVED(name, resolver, ctxptr) \
    static td_runtime_t name##_rt = {               \
        .cfg = NULL,                                \
        .resolve = (resolver),                      \
        .resolve_ctx = (ctxptr),                    \
    }

#define TD_ACTION(name)         \
    {                           \
        .fn = {                 \
            NULL,               \
            on_dance_finished,  \
            on_dance_reset,     \
            on_dance_release    \
        },                      \
        .user_data = &name##_rt \
    }

// ----------------------------------------------------------------------------
// Layers

#define RETURN_LAYER_NOT_SET 13

static uint8_t return_layer = RETURN_LAYER_NOT_SET;
static uint8_t return_layer_cnt = 0;

#define LAYER_PUSH(layer)                               \
    mouse_mode(false);                                  \
    if (return_layer_cnt == 0) {                        \
        return_layer = get_highest_layer(layer_state);  \
    }                                                   \
    return_layer_cnt++;                                 \
    layer_move(layer)

#define LAYER_RESTORE()                             \
    if (return_layer_cnt > 0) {                     \
        return_layer_cnt--;                         \
        if (return_layer_cnt == 0) {                \
            layer_move(return_layer);               \
            return_layer = RETURN_LAYER_NOT_SET;    \
        }                                           \
    }

// ----------------------------------------------------------------------------
// Shift Handling

static inline bool td_phys_shift(void) {
    return (get_mods() & MOD_MASK_SHIFT);
}

static inline bool td_logical_shift(void) {
    return (is_caps_word_on() || host_keyboard_led_state().caps_lock);
}

static inline bool td_effective_shift(void) {
    bool caps = td_logical_shift();
    bool phys = td_phys_shift();
    return caps ^ phys;
}

// ----------------------------------------------------------------------------
// Runtime helpers

static inline td_action_t td_action_none(void) {
    return (td_action_t){ .type = TD_ACT_NONE };
}

static void td_bind_cfg(td_runtime_t *rt) {
    if (rt->cfg || !rt->resolve)
        return;

    rt->cfg = rt->resolve(rt->resolve_ctx);
}

static td_depth_t td_compute_depth(const td_config_t *cfg) {
    td_depth_t depth = TD_DEPTH_SINGLE;

    for (uint8_t i = 0; i < cfg->len; i++) {
        switch (cfg->map[i].state) {
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

static bool td_can_emit_on_release(
    td_state_t state,
    const td_config_t *cfg
) {
    td_depth_t depth = td_compute_depth(cfg);

    switch (state) {
        case TD_SINGLE_TAP:
            return depth == TD_DEPTH_SINGLE;

        case TD_DOUBLE_TAP:
        case TD_DOUBLE_SINGLE_TAP:
            return depth <= TD_DEPTH_DOUBLE;

        case TD_TRIPLE_TAP:
        case TD_TRIPLE_SINGLE_TAP:
            return true;

        default:
            return false;
    }
}

static td_state_t td_dance_step(tap_dance_state_t *s) {
    if (s->count == 1) {
        return (s->interrupted || !s->pressed)
            ? TD_SINGLE_TAP
            : TD_SINGLE_HOLD;
    }

    if (s->count == 2) {
        if (s->interrupted) return TD_DOUBLE_SINGLE_TAP;
        return s->pressed ? TD_DOUBLE_HOLD : TD_DOUBLE_TAP;
    }

    if (s->count == 3) {
        if (s->interrupted) return TD_TRIPLE_SINGLE_TAP;
        return s->pressed ? TD_TRIPLE_HOLD : TD_TRIPLE_TAP;
    }

    return TD_UNKNOWN;
}

static td_action_t td_lookup_action(const td_runtime_t *rt, td_state_t state) {
    if (!rt->cfg)
        return td_action_none();

    for (uint8_t i = 0; i < rt->cfg->len; i++) {
        if (rt->cfg->map[i].state == state)
            return rt->cfg->map[i].action;
    }

    return td_action_none();
}

// ----------------------------------------------------------------------------
// Execute/Cleanup

static void td_execute(
    td_runtime_t *rt,
    td_action_t action,
    td_exec_origin_t origin
) {
    if (rt->active.active || action.type == TD_ACT_NONE)
        return;

    if (origin == TD_EXEC_RELEASE) {
        switch (action.type) {
            case TD_ACT_KC:
            case TD_ACT_KC_SFT:
                break;

            default:
                return;
        }
    }

    rt->active.action = action;
    rt->active.active = true;

    switch (action.type) {
        case TD_ACT_KC:
            rt->active.emitted_kc = td_effective_shift()
                ? LSFT(action.val.keycode)
                : action.val.keycode;

            register_code16(rt->active.emitted_kc);
            break;

        case TD_ACT_KC_SFT: {
            bool caps = td_logical_shift();
            bool phys = td_phys_shift();

            rt->active.emitted_kc = phys
                ? action.val.kc_pair.shifted
                : (phys ^ caps
                    ? LSFT(action.val.kc_pair.base)
                    : action.val.kc_pair.base);

            register_code16(rt->active.emitted_kc);
            break;
        }

        case TD_ACT_MOD:
            register_mods(MOD_BIT(action.val.keycode));
            break;

        case TD_ACT_LMV:
            LAYER_PUSH(action.val.layer);
            break;

        case TD_ACT_FN:
            if (action.val.fn)
                action.val.fn(action.ctx);
            break;

        default:
            break;
    }
}

static void td_cleanup(td_runtime_t *rt) {
    if (!rt->active.active)
        return;

    td_action_t *action = &rt->active.action;
    switch (action->type) {
        case TD_ACT_KC:
        case TD_ACT_KC_SFT:
            unregister_code16(rt->active.emitted_kc);
            mouse_mode(false);
            break;

        case TD_ACT_MOD:
            unregister_mods(MOD_BIT(action->val.keycode));
            break;

        case TD_ACT_LMV:
            LAYER_RESTORE();
            break;

        default:
            break;
    }

    if (action->reset_fn)
        action->reset_fn(action->ctx);

    rt->active.action = td_action_none();
    rt->active.active = false;

    if (rt->resolve)
        rt->cfg = NULL;
}

// ----------------------------------------------------------------------------
// Callbacks

static void on_dance_release(tap_dance_state_t *state, void *user_data) {
    td_runtime_t *rt = user_data;

    td_bind_cfg(rt);
    if (!rt->cfg)
        return;

    td_state_t td_state = td_dance_step(state);
    if (!td_can_emit_on_release(td_state, rt->cfg))
        return;

    td_action_t action = td_lookup_action(rt, td_state);
    td_execute(rt, action, TD_EXEC_RELEASE);

    if (rt->active.active)
        state->finished = true;
}

static void on_dance_finished(tap_dance_state_t *state, void *user_data) {
    td_runtime_t *rt = user_data;
    if (rt->active.active)
        return;

    td_bind_cfg(rt);
    if (!rt->cfg)
        return;

    td_state_t td_state = td_dance_step(state);
    td_action_t action = td_lookup_action(rt, td_state);

    td_execute(rt, action, TD_EXEC_FINISHED);
}

static void on_dance_reset(tap_dance_state_t *state, void *user_data) {
    (void)state;

    td_runtime_t *rt = user_data;
    td_cleanup(rt);
}

// ----------------------------------------------------------------------------
// HRM Resolver

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

#define TD_HRM(name, kc, mod, hand_)                    \
    TD_CFG(name##_enabled,                              \
        TD_MAP(TD_SINGLE_TAP, TD_KC(kc)),               \
        TD_MAP(TD_SINGLE_HOLD, TD_MOD(mod))             \
    );                                                  \
    TD_CFG(name##_disabled,                             \
        TD_MAP(TD_SINGLE_TAP, TD_KC(kc))                \
    );                                                  \
    static const hrm_td_ctx_t name##_ctx = {            \
        .hand = (hand_),                                \
        .enabled = &name##_enabled_cfg,                 \
        .disabled = &name##_disabled_cfg,               \
    };                                                  \
    TD_RUNTIME_RESOLVED(name, hrm_resolver, &name##_ctx)

// ----------------------------------------------------------------------------
// Definitions

TD_HRM(hrm_a, KC_A, KC_LGUI, HRM_HAND_LEFT);
TD_HRM(hrm_r, KC_R, KC_LALT, HRM_HAND_LEFT);
TD_HRM(hrm_s, KC_S, KC_LSFT, HRM_HAND_LEFT);
TD_HRM(hrm_t, KC_T, KC_LCTL, HRM_HAND_LEFT);

TD_HRM(hrm_n, KC_N, KC_RCTL, HRM_HAND_RIGHT);
TD_HRM(hrm_e, KC_E, KC_RSFT, HRM_HAND_RIGHT);
TD_HRM(hrm_i, KC_I, KC_LALT, HRM_HAND_RIGHT);
TD_HRM(hrm_o, KC_O, KC_RGUI, HRM_HAND_RIGHT);

TD_CFG(ckc_spc,
    TD_MAP(TD_SINGLE_TAP, TD_KC(KC_SPC)),
    TD_MAP(TD_SINGLE_HOLD, TD_LMV(_NUM))
);
TD_RUNTIME(ckc_spc, ckc_spc);

TD_CFG(ckc_tab,
    TD_MAP(TD_SINGLE_TAP, TD_KC(KC_TAB)),
    TD_MAP(TD_SINGLE_HOLD, TD_LMV(_SYM))
);
TD_RUNTIME(ckc_tab, ckc_tab);

TD_CFG(ckc_bspc,
    TD_MAP(TD_SINGLE_TAP, TD_KC_SFT(KC_BSPC, KC_DEL)),
    TD_MAP(TD_SINGLE_HOLD, TD_LMV(_NAV))
);
TD_RUNTIME(ckc_bspc, ckc_bspc);

// ----------------------------------------------------------------------------
// Table

USER_TAP_DANCE_TABLE(
    [TDE_HRM_A] = TD_ACTION(hrm_a),
    [TDE_HRM_R] = TD_ACTION(hrm_r),
    [TDE_HRM_S] = TD_ACTION(hrm_s),
    [TDE_HRM_T] = TD_ACTION(hrm_t),

    [TDE_HRM_N] = TD_ACTION(hrm_n),
    [TDE_HRM_E] = TD_ACTION(hrm_e),
    [TDE_HRM_I] = TD_ACTION(hrm_i),
    [TDE_HRM_O] = TD_ACTION(hrm_o),

    [TDE_CK_BSPC] = TD_ACTION(ckc_bspc),
    [TDE_CK_SPC] = TD_ACTION(ckc_spc),
    [TDE_CK_TAB] = TD_ACTION(ckc_tab),
);
