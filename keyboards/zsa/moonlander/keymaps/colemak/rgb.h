#pragma once

#include QMK_KEYBOARD_H
#include "layers.h"

#define RGB_LAYOUT( \
    k00, k01, k02, k03, k04, k05, k06,   k60, k61, k62, k63, k64, k65, k66, \
    k10, k11, k12, k13, k14, k15, k16,   k70, k71, k72, k73, k74, k75, k76, \
    k20, k21, k22, k23, k24, k25, k26,   k80, k81, k82, k83, k84, k85, k86, \
    k30, k31, k32, k33, k34, k35,             k91, k92, k93, k94, k95, k96, \
    k40, k41, k42, k43, k44,      k53,   kb3,      ka2, ka3, ka4, ka5, ka6, \
                        k50, k51, k52,   kb4, kb5, kb6 \
) { \
    { k00, k01, k02, k03, k04, k05, k06 }, \
    { k10, k11, k12, k13, k14, k15, k16 }, \
    { k20, k21, k22, k23, k24, k25, k26 }, \
    { k30, k31, k32, k33, k34, k35, { HSV_OFF } }, \
    { k40, k41, k42, k43, k44, { HSV_OFF }, { HSV_OFF } }, \
    { k50, k51, k52, k53, { HSV_OFF }, { HSV_OFF }, { HSV_OFF } }, \
\
    { k60, k61, k62, k63, k64, k65, k66 }, \
    { k70, k71, k72, k73, k74, k75, k76 }, \
    { k80, k81, k82, k83, k84, k85, k86 }, \
    { { HSV_OFF }, k91, k92, k93, k94, k95, k96 }, \
    { { HSV_OFF }, { HSV_OFF }, ka2, ka3, ka4, ka5, ka6 }, \
    { { HSV_OFF }, { HSV_OFF }, { HSV_OFF }, kb3, kb4, kb5, kb6  } \
}

#define LED_LAYERS 8
static const bool led_layers[] = {
    [CLMK] = true,
    [AROW] = true,
    [MDIA] = true,
    [QWRT] = true,
    [GAME] = true,
    [GMNM] = true,
    [SYMB] = true,
    [FUNC] = true,
};

typedef uint8_t ledmap[MATRIX_ROWS][MATRIX_COLS][3];
extern const ledmap ledmaps[];

typedef struct {
    hsv_t hsv;
    bool compiled;
} compiled_hsv;

#define XXXXXX { HSV_OFF }
#define ANIMTE { 1, 1, 1 } // animate
#define ______ { 2, 2, 2 } // passthrough

#define RED { HSV_RED }
#define CORAL { HSV_CORAL }
#define ORANGE { HSV_ORANGE }
#define GOLDEN { HSV_GOLDENROD }
#define GOLD { HSV_GOLD }
#define YELLOW { HSV_YELLOW }
#define CHART { HSV_CHARTREUSE }
#define GREEN { HSV_GREEN }
#define SPRING { HSV_SPRINGGREEN }
#define TURQ { HSV_TURQUOISE }
#define TEAL { HSV_TEAL }
#define CYAN { HSV_CYAN }
#define AZURE { HSV_AZURE }
#define BLUE { HSV_BLUE }
#define PURPLE { HSV_PURPLE }
#define MAGENT { HSV_MAGENTA }
#define PINK { HSV_PINK }
#define BLACK { HSV_BLACK }
