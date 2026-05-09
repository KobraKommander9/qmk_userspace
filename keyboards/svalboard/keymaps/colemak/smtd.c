#include "sm_td.h"
#include "keys.h"
#include "layers.h"
#include "hrm.h"

#define CUSTOM_TAP(tap_kc)      \
    SMTD_TAP_16(false, tap_kc); \
    mouse_mode(false)

#define CUSTOM_UNTAP(tap_kc)            \
    SMTD_UNREGISTER_16(false, tap_kc);  \
    mouse_mode(false)

#define SHIFT_ACTION(normal_action, shift_action)   \
    if (mods & MOD_MASK_SHIFT) {                    \
        shift_mod = mods & MOD_BIT_LSHIFT           \
            ? MOD_BIT_LSHIFT                        \
            : MOD_BIT_RSHIFT;                       \
        unregister_mods(MOD_MASK_SHIFT);            \
        shift_action;                               \
    } else {                                        \
        normal_action;                              \
    }

#define SHIFT_TAP(tap_kc, shift_kc)     \
    SHIFT_ACTION(                       \
        SMTD_TAP_16(false, tap_kc);     \
        register_mods(shift_mod),       \
        SMTD_TAP_16(false, shift_kc))

#define SHIFT_REGISTER(tap_kc, shift_kc)    \
    SHIFT_ACTION(                           \
        SMTD_REGISTER_16(false, tap_kc),    \
        SMTD_REGISTER_16(false, shift_kc);  \
        shift_registered = true)

#define SHIFT_UNREGISTER(tap_kc, shift_kc)      \
    if (shift_registered) {                     \
        shift_registered = false;               \
        SMTD_UNREGISTER_16(false, shift_kc);    \
        register_mods(shift_mod);               \
    } else {                                    \
        SMTD_UNREGISTER_16(false, tap_kc);      \
    }

#define CUSTOM_LT(macro_key, tap_kc, layer)     \
    SMTD_DANCE(macro_key,                       \
        NOTHING,                                \
        CUSTOM_TAP(tap_kc),                     \
        SMTD_LIMIT(1,                           \
            mouse_mode(false);                  \
            LAYER_PUSH(layer),                  \
            SMTD_REGISTER_16(false, tap_kc)),   \
        SMTD_LIMIT(1,                           \
            LAYER_RESTORE(),                    \
            CUSTOM_UNTAP(tap_kc)))

#define SHIFTED_LT(macro_key, tap_kc, shift_kc, layer)  \
    SMTD_DANCE(macro_key,                               \
        NOTHING,                                        \
        SHIFT_TAP(tap_kc, shift_kc);                    \
        mouse_mode(false),                              \
        SMTD_LIMIT(1,                                   \
            mouse_mode(false);                          \
            LAYER_PUSH(layer),                          \
            SHIFT_REGISTER(tap_kc, shift_kc)),          \
        SMTD_LIMIT(1,                                   \
            LAYER_RESTORE(),                            \
            SHIFT_UNREGISTER(tap_kc, shift_kc);         \
            mouse_mode(false)))

#define HRM_ACTIVE(hand, if_active, otherwise) \
    if (hrm_active(hand)) { if_active; } else { otherwise; }

#define HRM_SMTD_CASE(macro_key, tap_kc, mod_kc, hand)  \
    SMTD_DANCE(macro_key,                               \
        NOTHING,                                        \
        CUSTOM_TAP(tap_kc),                             \
        SMTD_LIMIT(1,                                   \
            HRM_ACTIVE(hand,                            \
                register_mods(MOD_BIT(mod_kc));         \
                send_keyboard_report(),                 \
                SMTD_REGISTER_16(false, tap_kc)),       \
            SMTD_REGISTER_16(false, tap_kc)),           \
        SMTD_LIMIT(1,                                   \
            HRM_ACTIVE(hand,                            \
                unregister_mods(MOD_BIT(mod_kc));       \
                send_keyboard_report(),                 \
                SMTD_UNREGISTER_16(false, tap_kc)),     \
            SMTD_UNREGISTER_16(false, tap_kc));         \
            send_keyboard_report())

smtd_resolution on_smtd_action(uint16_t keycode, smtd_action action, uint8_t tap_count) {
    static bool shift_registered = false;
    static uint8_t shift_mod = 0;
    const uint8_t mods = get_mods();

    switch (keycode) {
        SHIFTED_LT(CKC_BSPC, KC_BSPC, KC_DEL, _NAV)
        CUSTOM_LT(CKC_SPC, KC_SPC, _NUM)
        CUSTOM_LT(CKC_TAB, KC_TAB, _SYM)

        HRM_SMTD_CASE(HRM_A, KC_A, KC_LGUI, HRM_HAND_LEFT)
        HRM_SMTD_CASE(HRM_R, KC_R, KC_LALT, HRM_HAND_LEFT)
        HRM_SMTD_CASE(HRM_S, KC_S, KC_LSFT, HRM_HAND_LEFT)
        HRM_SMTD_CASE(HRM_T, KC_T, KC_LCTL, HRM_HAND_LEFT)

        HRM_SMTD_CASE(HRM_N, KC_N, KC_RCTL, HRM_HAND_RIGHT)
        HRM_SMTD_CASE(HRM_E, KC_E, KC_RSFT, HRM_HAND_RIGHT)
        HRM_SMTD_CASE(HRM_I, KC_I, KC_LALT, HRM_HAND_RIGHT)
        HRM_SMTD_CASE(HRM_O, KC_O, KC_RGUI, HRM_HAND_RIGHT)
    }

    return SMTD_RESOLUTION_UNHANDLED;
}
