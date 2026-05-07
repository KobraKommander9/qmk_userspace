LTO_ENABLE = yes # I don't remember what this does

AUDIO_ENABLE = no # disable keyboard sounds
CAPS_WORD_ENABLE = yes # enables caps word key
COMBO_ENABLE = yes # enables combos/chording
TAP_DANCE_ENABLE = yes # enables tap dances

SRC += combos.c
SRC += layers.c
SRC += keys.c
SRC += rgb.c
SRC += tap_dance.c
