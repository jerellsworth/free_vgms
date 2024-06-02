#ifndef MAIN_H
#define MAIN_H

#include <genesis.h>

#include "resources.h"
#include "songs.h"

typedef enum {
    STOPPED,
    PLAYING
} SongState;

typedef struct {
    u16 song_idx;
    u16 song_idx_playing;
    SongState song_state;
    u16 input_cooldown;
} State;

#endif
