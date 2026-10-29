CAPS_WORD_ENABLE = yes
LAYER_LOCK_ENABLE = yes

VIA_ENABLE = yes

VPATH += keyboards/svalboard/keymaps
SRC += keymap_support.c

SRC += persist.c
SRC += hrm.c
SRC += layers.c
SRC += custom_keys.c
SRC += macros.c

# DO NOT ENABLE LTO — Svalboard will fail to compile with it
# LTO_ENABLE = yes
