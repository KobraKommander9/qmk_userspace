VIA_ENABLE = yes
VIAL_ENABLE = yes
VIAL_INSECURE ?= yes

VPATH += keyboards/svalboard/keymaps
SRC += keymap_support.c

SRC += hrm.c
SRC += layers.c

# DO NOT ENABLE LTO — Svalboard will fail to compile with it
# LTO_ENABLE = yes
