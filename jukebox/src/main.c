#include "main.h"

void *ct_calloc(u16 nitems, u16 size) {
    u16 bytes = nitems * size;
    void *p = malloc(bytes);
    if (!p) {
        VDP_drawTextBG(BG_A, "NULL POINTER RETURNED FROM MALLOC", 1, 1);
        while (TRUE) {
            SYS_doVBlankProcess();
            VDP_waitVSync();
        }
    }
    memset(p, 0, bytes);
    return p;
}

void update_display(State *s) {
    VDP_clearTextArea(0, 0, 40, 28);    
    if (s->song_state == PLAYING) {
        VDP_drawText("Playing:", 0, 0);
        VDP_drawText(XGM_TITLES[s->song_idx_playing], 0, 1);
    } else {
        VDP_drawText("Stopped", 0, 0);
    }
    VDP_drawText(XGM_TITLES[s->song_idx], 0, 15);
}

void change_song_state(State *s) {
    if (s->song_state == STOPPED) {
        s->song_idx_playing = s->song_idx;
        s->song_state = PLAYING;
        XGM_startPlay(XGM_PTRS[s->song_idx_playing]);
        update_display(s);
        return;
    }
    if (s->song_state == PLAYING) {
        if (s->song_idx != s->song_idx_playing) {
            s->song_idx_playing = s->song_idx;
            XGM_startPlay(XGM_PTRS[s->song_idx_playing]);
            update_display(s);
            return;
        }
        s->song_state = STOPPED;
        XGM_stopPlay();
        update_display(s);
        return;
    }
}

void change_song_selected(State *s, s16 delta) {
    if (s->song_idx == 0 && delta < 0) {
        s->song_idx = NUM_XGMS - 1;
    } else if (s->song_idx >= NUM_XGMS - 1 && delta > 0) {
        s->song_idx = 0;
    } else {
        s->song_idx += delta;
    }
    update_display(s);
}

void player_input(State *s) {
    if (s->input_cooldown > 0) {
        s->input_cooldown -= 1;
        return;
    }
    u16 joy = JOY_readJoypad(JOY_ALL);
    if (joy & (BUTTON_LEFT | BUTTON_UP)) {
        change_song_selected(s, -1);
        s->input_cooldown = 10;
        return;
    }
    if (joy & (BUTTON_RIGHT | BUTTON_DOWN)) {
        change_song_selected(s, 1);
        s->input_cooldown = 10;
        return;
    }
    if (joy & (BUTTON_A | BUTTON_B | BUTTON_C)) {
        change_song_state(s);
        s->input_cooldown = 10;
        return;
    }
}

int main(bool hard_reset) {
    if (!hard_reset) SYS_hardReset();

    SPR_init();
    JOY_init();

    State *s = ct_calloc(1, sizeof(State));
    s->song_idx = 0;
    s->song_idx_playing = 0;
    s->song_state = STOPPED;
    update_display(s);

    while (TRUE) {
        player_input(s);
        if ((!XGM_isPlaying()) && (s->song_state == PLAYING)) {
            XGM_startPlay(XGM_PTRS[s->song_idx_playing]);
        }
        SYS_doVBlankProcess();
    }
	return 0;
}
