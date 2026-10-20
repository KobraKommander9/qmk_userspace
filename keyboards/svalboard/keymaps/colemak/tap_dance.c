#include "tap_dance.h"

#include "hrm.h"
#include "layers.h"
#include "keys.h"
#include "shift.h"

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

    TD_MORE_TAPS,
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
    TD_ACT_TAP_HOLD,
    TD_ACT_MOD,
    TD_ACT_LMV,
    TD_ACT_LT,
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
    bool hold_on_interrupt;
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
    bool rapid;
} td_runtime_t;

// ----------------------------------------------------------------------------
// Action constructors

#define OVERLOAD3(_1, _2, _3, NAME, ...) NAME

#define TD_KC(kc)               \
    ((td_action_t){             \
        .type = TD_ACT_KC,      \
        .val.keycode = (kc),    \
    })

#define TD_TAP_HOLD(kc)             \
    ((td_action_t){                 \
        .type = TD_ACT_TAP_HOLD,    \
        .val.keycode = (kc),        \
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

#define TD_LMV(layer_)          \
    ((td_action_t){             \
        .type = TD_ACT_LMV,     \
        .val.layer = (layer_),  \
    })

#define TD_LT(layer_)           \
    ((td_action_t){             \
        .type = TD_ACT_LT,      \
        .val.layer = (layer_),  \
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

#define TD_CFG_HOI(name, ...)                       \
    static const td_action_map_t name##_map[] = {   \
        __VA_ARGS__                                 \
    };                                              \
    static const td_config_t name##_cfg = {         \
        .map = name##_map,                          \
        .len = ARRAY_SIZE(name##_map),              \
        .hold_on_interrupt = true,                  \
    }

#define TD_RUNTIME(name)                \
    static td_runtime_t name##_rt = {   \
        .cfg = &name##_cfg,             \
    }

#define TD_RUNTIME_CFG(name, cfgname)   \
    static td_runtime_t name##_rt = {   \
        .cfg = &cfgname##_cfg,          \
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
            on_dance_each_tap,  \
            on_dance_finished,  \
            on_dance_reset,     \
            on_dance_release    \
        },                      \
        .user_data = &name##_rt \
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

    return TD_MORE_TAPS;
}

static td_action_t td_lookup_action(const td_runtime_t *rt, td_state_t state) {
    if (!rt->cfg)
        return td_action_none();

    const td_config_t *cfg = rt->cfg;
    for (uint8_t i = 0; i < cfg->len; i++) {
        if (cfg->map[i].state == state)
            return cfg->map[i].action;
    }

    return td_action_none();
}

// Depth based only on tap-type states (used for on_each_tap overflow threshold)
static td_depth_t td_tap_depth(const td_config_t *cfg) {
    td_depth_t depth = TD_DEPTH_SINGLE;

    for (uint8_t i = 0; i < cfg->len; i++) {
        td_state_t s = cfg->map[i].state;

        if (s == TD_TRIPLE_TAP || s == TD_TRIPLE_SINGLE_TAP) return TD_DEPTH_TRIPLE;
        if (s == TD_DOUBLE_TAP || s == TD_DOUBLE_SINGLE_TAP) depth = TD_DEPTH_DOUBLE;
    }

    return depth;
}

// True if any multi-tap (non-hold) actions defined — blocks immediate on_dance_release
static bool td_has_multi_tap(const td_config_t *cfg) {
    for (uint8_t i = 0; i < cfg->len; i++) {
        td_state_t s = cfg->map[i].state;

        if (s == TD_DOUBLE_TAP || s == TD_DOUBLE_SINGLE_TAP ||
            s == TD_TRIPLE_TAP || s == TD_TRIPLE_SINGLE_TAP)
            return true;
    }

    return false;
}

// ----------------------------------------------------------------------------
// Execute/Cleanup

static void td_emit_kc16(td_runtime_t *rt, uint16_t kc, bool hold, bool suppress) {
    rt->active.emitted_kc = kc;
    if (hold) {
        register_code16(kc);
    } else {
        if (suppress) { del_mods(MOD_MASK_SHIFT); send_keyboard_report(); }
        tap_code16(kc);
        if (suppress) { add_mods(MOD_MASK_SHIFT); send_keyboard_report(); }
        mouse_mode(false);
    }
}

static void td_emit(td_runtime_t *rt, td_action_t action, bool hold) {
    rt->active.action = action;
    rt->active.active = true;

    switch (action.type) {
        case TD_ACT_KC: {
            shift_state_t sh = get_shift_state();
            break_caps_word(action.val.keycode);
            uint16_t kc = sh.effective ? LSFT(action.val.keycode) : action.val.keycode;
            td_emit_kc16(rt, kc, hold, sh.phys && sh.caps);
            break;
        }

        case TD_ACT_KC_SFT: {
            shift_state_t sh = get_shift_state();
            uint16_t kc = sh.phys ? action.val.kc_pair.shifted : action.val.kc_pair.base;
            break_caps_word(kc);
            td_emit_kc16(rt, sh.caps && !sh.phys ? LSFT(kc) : kc, hold, sh.phys);
            break;
        }

        case TD_ACT_TAP_HOLD:
            rt->active.emitted_kc = action.val.keycode;
            tap_code16(action.val.keycode);
            mouse_mode(false);
            register_code16(action.val.keycode);
            break;

        case TD_ACT_MOD:
            if (hold) {
                break_caps_word(action.val.keycode);
                register_mods(MOD_BIT(action.val.keycode));
            }

            break;

        case TD_ACT_LMV:
            layer_move(action.val.layer);
            mouse_mode(false);
            break;

        case TD_ACT_LT:
            layer_on(action.val.layer);
            mouse_mode(false);
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
        case TD_ACT_TAP_HOLD:
            unregister_code16(rt->active.emitted_kc);
            mouse_mode(false);
            break;

        case TD_ACT_MOD:
            unregister_mods(MOD_BIT(action->val.keycode));
            mouse_mode(false);
            break;

        case TD_ACT_LT:
            layer_off(action->val.layer);
            mouse_mode(false);
            break;

        default:
            break;
    }

    if (action->reset_fn)
        action->reset_fn(action->ctx);

    rt->active.active = false;
    rt->active.action = td_action_none();

    if (rt->resolve)
        rt->cfg = NULL;
}

// ----------------------------------------------------------------------------
// Callbacks

static void on_dance_each_tap(tap_dance_state_t *state, void *user_data) {
    td_runtime_t *rt = user_data;
    td_bind_cfg(rt);
    if (!rt->cfg) return;

    td_depth_t depth = td_tap_depth(rt->cfg);

    // SINGLE-depth: on_dance_release emits one char per key-up; nothing to do here
    if (depth == TD_DEPTH_SINGLE) return;

    uint8_t threshold = (depth == TD_DEPTH_TRIPLE) ? 3 : 2;

    if (state->count <= threshold) return;

    // on_dance_release suppressed for multi-tap configs; emit retroactively on first overflow
    td_action_t action = td_lookup_action(rt, TD_SINGLE_TAP);

    uint16_t kc;
    if (action.type == TD_ACT_KC) {
        kc = action.val.keycode;
    } else if (action.type == TD_ACT_KC_SFT) {
        shift_state_t sh = get_shift_state();
        kc = sh.phys ? action.val.kc_pair.shifted : action.val.kc_pair.base;
    } else {
        return;
    }

    if (state->count == threshold + 1) {
        for (uint8_t i = 0; i < state->count; i++) tap_code16(kc);
    } else {
        tap_code16(kc);
    }

    rt->rapid = true;
}

static void on_dance_release(tap_dance_state_t *state, void *user_data) {
    td_runtime_t *rt = user_data;
    if (rt->active.active) return;
    if (rt->rapid) return;

    td_bind_cfg(rt);
    if (!rt->cfg) return;

    // Multi-tap (tap-type doubles/triples) configs wait for on_dance_finished
    if (td_has_multi_tap(rt->cfg)) return;

    // Emit single-tap keycode on every non-hold release:
    // count=1 → immediate tap; count=2,3,... → one more per rapid press
    td_state_t td_state = td_dance_step(state);
    bool is_hold = (td_state == TD_SINGLE_HOLD ||
                    td_state == TD_DOUBLE_HOLD  ||
                    td_state == TD_TRIPLE_HOLD);
    if (is_hold) return;

    td_action_t action = td_lookup_action(rt, TD_SINGLE_TAP);
    td_emit(rt, action, false);
    rt->active.active = false;
}

static void on_dance_finished(tap_dance_state_t *state, void *user_data) {
    td_runtime_t *rt = user_data;

    if (rt->rapid) { rt->rapid = false; return; }

    td_bind_cfg(rt);
    if (!rt->cfg) return;

    td_state_t td_state = td_dance_step(state);

    if (rt->cfg->hold_on_interrupt && state->interrupted && state->pressed) {
        if      (td_state == TD_SINGLE_TAP)        td_state = TD_SINGLE_HOLD;
        else if (td_state == TD_DOUBLE_SINGLE_TAP) td_state = TD_DOUBLE_HOLD;
        else if (td_state == TD_TRIPLE_SINGLE_TAP) td_state = TD_TRIPLE_HOLD;
    }

    bool is_hold = (td_state == TD_SINGLE_HOLD ||
                    td_state == TD_DOUBLE_HOLD  ||
                    td_state == TD_TRIPLE_HOLD);

    // SINGLE-depth tap states handled by on_dance_release (responsive path)
    if (!is_hold && !td_has_multi_tap(rt->cfg)) return;

    td_action_t action = td_lookup_action(rt, td_state);
    if (action.type == TD_ACT_NONE) return;

    td_emit(rt, action, is_hold);

    if (!is_hold && action.type != TD_ACT_TAP_HOLD)
        rt->active.active = false;
}

static void on_dance_reset(tap_dance_state_t *state, void *user_data) {
    td_runtime_t *rt = user_data;
    td_release(rt);
}

// ----------------------------------------------------------------------------
// Definitions

TD_CFG(ckc_bspc,
    TD_MAP(TD_SINGLE_TAP,  TD_KC_SFT(KC_BSPC, KC_DEL)),
    TD_MAP(TD_SINGLE_HOLD, TD_LT(_NAV)),
    TD_MAP(TD_DOUBLE_HOLD, TD_KC_SFT(KC_BSPC, KC_DEL))
);
TD_RUNTIME(ckc_bspc);

TD_CFG(ckc_exlm,
    TD_MAP(TD_SINGLE_TAP, TD_KC_SFT(KC_EXLM, KC_AMPR)),
    TD_MAP(TD_DOUBLE_TAP, TD_KC_SFT(KC_ASTR, KC_HASH)),
);
TD_RUNTIME(ckc_exlm);

TD_CFG(ckc_game,
    TD_MAP(TD_SINGLE_TAP, TD_KC(KC_TAB)),
    TD_MAP(TD_DOUBLE_TAP, TD_LT(_GAME1))
);
TD_RUNTIME(ckc_game);

TD_CFG(ckc_grv,
    TD_MAP(TD_SINGLE_TAP, TD_KC(KC_GRV)),
    TD_MAP(TD_DOUBLE_TAP, TD_KC(KC_PERC)),
);
TD_RUNTIME(ckc_grv);

TD_CFG(ckc_hypr,
    TD_MAP(TD_SINGLE_TAP, TD_KC(KC_LALT)),
    TD_MAP(TD_DOUBLE_TAP, TD_KC(KC_HYPR)),
    TD_MAP(TD_DOUBLE_HOLD, TD_KC(KC_HYPR))
);
TD_RUNTIME(ckc_hypr);

TD_CFG(ckc_media,
    TD_MAP(TD_DOUBLE_TAP, TD_LT(_MEDIA)),
    TD_MAP(TD_TRIPLE_TAP, TD_KC(HRM_TOGGLE))
);
TD_RUNTIME(ckc_media);

TD_CFG(ckc_meh,
    TD_MAP(TD_SINGLE_TAP, TD_KC(KC_LCTL)),
    TD_MAP(TD_DOUBLE_TAP, TD_KC(KC_MEH)),
    TD_MAP(TD_DOUBLE_HOLD, TD_KC(KC_MEH))
);
TD_RUNTIME(ckc_meh);

TD_CFG(ckc_spc,
    TD_MAP(TD_SINGLE_TAP,  TD_KC(KC_SPC)),
    TD_MAP(TD_SINGLE_HOLD, TD_LT(_NUM)),
    TD_MAP(TD_DOUBLE_HOLD, TD_KC(KC_SPC))
);
TD_RUNTIME(ckc_spc);

TD_CFG(ckc_tab,
    TD_MAP(TD_SINGLE_TAP,  TD_KC(KC_TAB)),
    TD_MAP(TD_SINGLE_HOLD, TD_LT(_SYM)),
    TD_MAP(TD_DOUBLE_HOLD, TD_KC(KC_TAB))
);
TD_RUNTIME(ckc_tab);

// ----------------------------------------------------------------------------
// Table

tap_dance_action_t tap_dance_actions[] = {
    [TDE_CK_BSPC] = TD_ACTION(ckc_bspc),
    [TDE_CK_EXLM] = TD_ACTION(ckc_exlm),
    [TDE_CK_GAME] = TD_ACTION(ckc_game),
    [TDE_CK_GRV] = TD_ACTION(ckc_grv),
    [TDE_CK_HYPR] = TD_ACTION(ckc_hypr),
    [TDE_CK_MEDIA] = TD_ACTION(ckc_media),
    [TDE_CK_MEH] = TD_ACTION(ckc_meh),
    [TDE_CK_SPC] = TD_ACTION(ckc_spc),
    [TDE_CK_TAB] = TD_ACTION(ckc_tab),
};
