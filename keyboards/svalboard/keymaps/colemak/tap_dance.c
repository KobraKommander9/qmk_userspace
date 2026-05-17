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

typedef struct {
    uint16_t kc;
    bool active;
} td_press_t;

typedef struct {
    bool hold;
} td_commit_t;

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

typedef struct {
    bool phys;
    bool caps;
    bool effective;
} td_shift_t;

static inline td_shift_t td_get_shift(void) {
    bool phys = (get_mods() & MOD_MASK_SHIFT);
    bool caps  = is_caps_word_on() || host_keyboard_led_state().caps_lock;

    return (td_shift_t){
        .phys = phys,
        .caps = caps,
        .effective = phys ^ caps,
    };
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

static td_state_t td_get_fallback(td_state_t state) {
    switch (state) {
        case TD_SINGLE_HOLD: return TD_SINGLE_TAP;
        case TD_DOUBLE_TAP: return TD_SINGLE_TAP;
        case TD_DOUBLE_HOLD: return TD_DOUBLE_TAP;
        case TD_DOUBLE_SINGLE_TAP: return TD_DOUBLE_TAP;
        case TD_TRIPLE_TAP: return TD_DOUBLE_TAP;
        case TD_TRIPLE_HOLD: return TD_TRIPLE_TAP;
        case TD_TRIPLE_SINGLE_TAP: return TD_TRIPLE_TAP;
        default: return TD_UNKNOWN;
    }
}

static td_action_t td_lookup_action(const td_runtime_t *rt, td_state_t state) {
    if (!rt->cfg)
        return td_action_none();

    const td_config_t *cfg = rt->cfg;
    td_state_t candidate = state;

    while (candidate != TD_UNKNOWN) {
        for (uint8_t i = 0; i < cfg->len; i++) {
            if (cfg->map[i].state == candidate)
                return cfg->map[i].action;
        }
        candidate = td_get_fallback(candidate);
    }

    return td_action_none();
}

// ----------------------------------------------------------------------------
// Execute/Cleanup

static uint16_t td_resolve_keycode(td_action_t *action, td_shift_t sh) {
    switch (action->type) {
        case TD_ACT_KC:
            return sh.effective
                ? LSFT(action->val.keycode)
                : action->val.keycode;

        case TD_ACT_KC_SFT:
            if (sh.phys)
                return action->val.kc_pair.shifted;

            return sh.caps
                ? LSFT(action->val.kc_pair.base)
                : action->val.kc_pair.base;

        default:
            return 0;
    }
}

static void td_emit(td_runtime_t *rt, td_action_t action, bool hold)
{
    td_shift_t sh = td_get_shift();

    rt->active.action = action;
    rt->active.active = true;

    switch (action.type) {
        case TD_ACT_KC:
        case TD_ACT_KC_SFT: {
            uint16_t kc = td_resolve_keycode(&action, sh);
            if (!kc) return;

            rt->active.emitted_kc = kc;

            if (hold) {
                register_code16(kc);
            } else {
                tap_code16(kc);
                mouse_mode(false);
            }

            break;
        }

        case TD_ACT_MOD:
            if (hold) {
                register_mods(MOD_BIT(action.val.keycode));
            } else {
                tap_code16(MOD_BIT(action.val.keycode));
                mouse_mode(false);
            }

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

static void td_release(td_runtime_t *rt) {
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
            mouse_mode(false);
            break;

        case TD_ACT_LMV:
            LAYER_RESTORE();
            break;

        default:
            break;
    }

    rt->active.active = false;
    rt->active.action = td_action_none();

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
    if (td_state == TD_SINGLE_TAP ||
        td_state == TD_DOUBLE_TAP ||
        td_state == TD_TRIPLE_TAP ||
        td_state == TD_DOUBLE_SINGLE_TAP ||
        td_state == TD_TRIPLE_SINGLE_TAP) {

        td_action_t action = td_lookup_action(rt, td_state);
        td_emit(rt, action, false);
    }
}

static void on_dance_finished(tap_dance_state_t *state, void *user_data) {
    td_runtime_t *rt = user_data;
    if (rt->active.active)
        return;

    td_bind_cfg(rt);
    if (!rt->cfg)
        return;

    td_state_t td_state = td_dance_step(state);
    if (td_state == TD_SINGLE_HOLD ||
        td_state == TD_DOUBLE_HOLD ||
        td_state == TD_TRIPLE_HOLD) {

        td_action_t action = td_lookup_action(rt, td_state);
        td_emit(rt, action, true);
    }
}

static void on_dance_reset(tap_dance_state_t *state, void *user_data) {
    (void)state;

    td_runtime_t *rt = user_data;
    td_release(rt);
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
