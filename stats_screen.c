#include "assets.h"
#include "logic.h"
#pragma bank 4
#include <gb/gb.h>
#include "constants.h"
#include "graphics.h"
#include "game_state.h"
#include "missions.h"
#include "lang.h"

extern void nb_play_sound_build_b4(void);
extern void nb_stats_draw_index(uint8_t cursor_pos);
extern void nb_stats_draw_page(uint8_t page);

void stats_screen() {
uint8_t stats_page = 0;
uint8_t in_index   = 1;
uint8_t cursor_pos = 0;
uint8_t menu_running = 1;
uint8_t redraw = 1;
uint8_t joy;
const uint8_t index_to_page[7] = {0, 1, 2, 4, 5, 6, 7};

waitpadup();
move_win(7, 0);
clear_entire_window();
nb_draw_menu_border();

while (menu_running) {
    if (redraw == 1) {
        move_sprite(0, 0, 0);
        { uint8_t si; for (si = 1u; si <= 7u; si++) move_sprite(si, 0u, 0u); }
        if (in_index == 1) nb_stats_draw_index(cursor_pos);
        else               nb_stats_draw_page(stats_page);
        redraw = 0;
    }

    joy = joypad();

    if (in_index == 1) {
        if (joy & J_B) { menu_running = 0; waitpadup(); }
        if (joy & J_DOWN) {
            cursor_pos++;
            if (cursor_pos >= 7) cursor_pos = 0;
            nb_play_sound_build_b4(); redraw = 1; waitpadup();
        }
        if (joy & J_UP) {
            if (cursor_pos == 0) cursor_pos = 6;
            else cursor_pos--;
            nb_play_sound_build_b4(); redraw = 1; waitpadup();
        }
        if (joy & J_A) {
            stats_page = index_to_page[cursor_pos];
            in_index = 0; redraw = 1; waitpadup();
        }
    } else {
        if (joy & J_B) { in_index = 1; redraw = 1; waitpadup(); }
        if (joy & J_RIGHT) {
            if (stats_page == 2) { stats_page = 3; nb_play_sound_build_b4(); redraw = 1; waitpadup(); }
            else if (stats_page == 7) { stats_page = 8; nb_play_sound_build_b4(); redraw = 1; waitpadup(); }
        }
        if (joy & J_LEFT) {
            if (stats_page == 3) { stats_page = 2; nb_play_sound_build_b4(); redraw = 1; waitpadup(); }
            else if (stats_page == 8) { stats_page = 7; nb_play_sound_build_b4(); redraw = 1; waitpadup(); }
        }
    }

    wait_vbl_done();
}

// Sortie de l'Almanach
{ uint8_t si; for (si = 1u; si <= 7u; si++) move_sprite(si, 0u, 0u); }
set_sprite_tile(1u, 1u); set_sprite_tile(2u, 2u); set_sprite_tile(3u, 3u); set_sprite_tile(4u, 4u);
}
