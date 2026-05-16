CAPS_WORD_ENABLE = yes
TAP_DANCE_ENABLE = yes

CONSOLE_ENABLE = yes

VIA_ENABLE = yes
VIAL_ENABLE = yes
VIAL_INSECURE ?= yes

VPATH += keyboards/svalboard/keymaps
SRC += keymap_support.c

SRC += persist.c
SRC += hrm.c
SRC += layers.c
SRC += tap_dance.c

# DO NOT ENABLE LTO — Svalboard will fail to compile with it
# LTO_ENABLE = yes
