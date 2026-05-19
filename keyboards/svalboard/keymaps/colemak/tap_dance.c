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

#define TD_RUNTIME(name, cfgname)       \
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
            NULL,               \
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

static td_state_t td_get_fallback(td_state_t state) {
    switch (state) {
        case TD_SINGLE_HOLD: return TD_SINGLE_TAP;
        case TD_DOUBLE_TAP: return TD_SINGLE_TAP;
        case TD_DOUBLE_HOLD: return TD_DOUBLE_TAP;
        case TD_DOUBLE_SINGLE_TAP: return TD_DOUBLE_TAP;
        case TD_TRIPLE_TAP: return TD_DOUBLE_TAP;
        case TD_TRIPLE_HOLD: return TD_TRIPLE_TAP;
        case TD_TRIPLE_SINGLE_TAP: return TD_TRIPLE_TAP;
        case TD_MORE_TAPS: return TD_TRIPLE_TAP;
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

static void on_dance_release(tap_dance_state_t *state, void *user_data) {
    td_runtime_t *rt = user_data;
    if (rt->active.active) return;

    td_bind_cfg(rt);
    if (!rt->cfg)
        return;

    td_state_t td_state = td_dance_step(state);
    if (td_state == TD_SINGLE_TAP ||
        td_state == TD_DOUBLE_TAP ||
        td_state == TD_TRIPLE_TAP ||
        td_state == TD_DOUBLE_SINGLE_TAP ||
        td_state == TD_TRIPLE_SINGLE_TAP ||
        td_state == TD_MORE_TAPS) {

        td_action_t action = td_lookup_action(rt, td_state);
        td_emit(rt, action, false);
        rt->active.active = false;  // tap_code16 is synchronous; no release tracking needed
    }
}

static void on_dance_finished(tap_dance_state_t *state, void *user_data) {
    td_runtime_t *rt = user_data;

    td_bind_cfg(rt);
    if (!rt->cfg)
        return;

    td_state_t td_state = td_dance_step(state);

    // Hold-on-interrupt: if held while another key pressed, fire hold immediately
    if (state->interrupted && state->pressed) {
        if      (td_state == TD_SINGLE_TAP)        td_state = TD_SINGLE_HOLD;
        else if (td_state == TD_DOUBLE_SINGLE_TAP) td_state = TD_DOUBLE_HOLD;
        else if (td_state == TD_TRIPLE_SINGLE_TAP) td_state = TD_TRIPLE_HOLD;
    }

    if (td_state != TD_SINGLE_HOLD &&
        td_state != TD_DOUBLE_HOLD &&
        td_state != TD_TRIPLE_HOLD &&
        td_state != TD_MORE_TAPS)
        return;

    // Hold can override an intermediate tap already emitted by on_each_release
    // (e.g. first release of a double-hold). Prior emit was tap_code16 (already up),
    // so just clear the tracking flag — no unregister needed.
    rt->active.active = false;

    td_action_t action = td_lookup_action(rt, td_state);
    td_emit(rt, action, true);
}

static void on_dance_reset(tap_dance_state_t *state, void *user_data) {
    td_runtime_t *rt = user_data;
    td_release(rt);
}

// ----------------------------------------------------------------------------
// Definitions

TD_CFG(ckc_spc,
    TD_MAP(TD_SINGLE_TAP, TD_KC(KC_SPC)),
    TD_MAP(TD_SINGLE_HOLD, TD_LT(_NUM))
);
TD_RUNTIME(ckc_spc, ckc_spc);

TD_CFG(ckc_tab,
    TD_MAP(TD_SINGLE_TAP, TD_KC(KC_TAB)),
    TD_MAP(TD_SINGLE_HOLD, TD_LT(_SYM))
);
TD_RUNTIME(ckc_tab, ckc_tab);

TD_CFG(ckc_bspc,
    TD_MAP(TD_SINGLE_TAP,  TD_KC_SFT(KC_BSPC, KC_DEL)),
    TD_MAP(TD_SINGLE_HOLD, TD_LT(_NAV))
);
TD_RUNTIME(ckc_bspc, ckc_bspc);

// ----------------------------------------------------------------------------
// Table

USER_TAP_DANCE_TABLE(
    [TDE_CK_BSPC] = TD_ACTION(ckc_bspc),
    [TDE_CK_SPC] = TD_ACTION(ckc_spc),
    [TDE_CK_TAB] = TD_ACTION(ckc_tab),
);
