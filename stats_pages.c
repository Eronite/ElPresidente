#pragma bank 7
#include <gb/gb.h>
#include "constants.h"
#include "graphics.h"
#include "game_state.h"
#include "missions.h"
#include "lang.h"

extern void nb_story_get_current_step_b2(MissionStep *out);
extern void restore_shop_tiles(void);

// Efface la zone de contenu intérieure (cols 1-18, rows 1-15)
static void clear_inner(void) {
    uint8_t blank[18] = {0};
    uint8_t ri;
    for (ri = 1u; ri < 16u; ri++) set_win_tiles(1, ri, 18, 1, blank);
}

// --- SOUS-MENU INDEX ---
void stats_draw_index(uint8_t cursor_pos) {
    const char *index_titles[7];
    uint8_t irow;
    if (game.language == LANG_EN) {
        index_titles[0] = "General";
        index_titles[1] = "Social";
        index_titles[2] = "Income";
        index_titles[3] = "Goals";
        index_titles[4] = "Electricity";
        index_titles[5] = "Expenses";
        index_titles[6] = "Happiness";
    } else {
        index_titles[0] = "G~n~ral";
        index_titles[1] = "Social";
        index_titles[2] = "Revenus";
        index_titles[3] = "Objectifs";
        index_titles[4] = "Electricit~";
        index_titles[5] = "D~penses";
        index_titles[6] = "Bonheur";
    }
    clear_inner();
    draw_text(5, 1, "Almanach", 1);
    for (irow = 0; irow < 7; irow++) {
        uint8_t y = 3 + irow * 2;
        if (irow == cursor_pos) draw_text(1, y, ">", 1);
        draw_text(3, y, (char*)index_titles[irow], 1);
    }
}

// --- PAGE 0 : GENERAL ---
void stats_draw_page0(void) {
    clear_inner();
    if (game.language == LANG_EN) {
        draw_text(6, 1, "General", 1);
        draw_text(1, 5, "Date :", 1);
        { uint8_t mo = game.month; char mm[3]; mm[0]='0'+mo/10; mm[1]='0'+mo%10; mm[2]='\0'; draw_text(12, 5, mm, 1); }
        draw_text(14, 5, "/", 1);
        draw_number(15, 5, game.year, 1);
        draw_text(1, 7, "Population :", 1);
        draw_number(12, 7, game.population, 1);
        draw_text(1, 9, "Happiness :", 1);
        draw_number(12, 9, game.avg_happiness, 1);
        draw_text(1, 11, "Money :", 1);
        draw_number(12, 11, (game.money > 999999L ? 999999L : game.money), 1);
    } else {
        draw_text(6, 1, "G~n~ral", 1);
        draw_text(1, 5, "Date :", 1);
        { uint8_t mo = game.month; char mm[3]; mm[0]='0'+mo/10; mm[1]='0'+mo%10; mm[2]='\0'; draw_text(12, 5, mm, 1); }
        draw_text(14, 5, "/", 1);
        draw_number(15, 5, game.year, 1);
        draw_text(1, 7, "Population :", 1);
        draw_number(12, 7, game.population, 1);
        draw_text(1, 9, "Bonheur : ", 1);
        draw_number(12, 9, game.avg_happiness, 1);
        draw_text(1, 11, "Argent :", 1);
        draw_number(12, 11, (game.money > 999999L ? 999999L : game.money), 1);
    }
}

// --- PAGE 1 : SOCIAL ---
void stats_draw_page1(void) {
    clear_inner();
    if (game.language == LANG_EN) {
        draw_text(6, 1, "Social", 1);
        draw_text(1, 5, "Homeless :", 1);
        draw_number(15, 5, game.homeless, 1);
        draw_text(1, 7, "Crime rate :", 1);
        draw_number(15, 7, game.crime_rate, 1);
        draw_text(1, 9, "Unemployment :", 1);
        draw_number(15, 9, game.unemployment_rate, 1);
    } else {
        draw_text(6, 1, "Social", 1);
        draw_text(1, 5, "Sans abris :", 1);
        draw_number(15, 5, game.homeless, 1);
        draw_text(1, 7, "Taux crime :", 1);
        draw_number(15, 7, game.crime_rate, 1);
        draw_text(1, 9, "Taux chomage :", 1);
        draw_number(15, 9, game.unemployment_rate, 1);
        draw_text(1, 11, "Nourriture", 1);
        draw_text(1, 12, "produite    :", 1);
        draw_number(15, 12, game.foodProduction, 1);
        draw_text(1, 13, "consomm~e   :", 1);
        draw_number(15, 13, game.foodConsumption, 1);
    }
}

// --- PAGE 2 : REVENUS 1/2 ---
void stats_draw_page2(void) {
    clear_inner();
    if (game.language == LANG_EN) {
        draw_text(4, 1, "Income 1/2", 1);
        draw_text(1, 4, "Rents :", 1);
        draw_number(15, 4, game.rev_rents, 1);
        draw_text(1, 6, "Food :", 1);
        draw_number(15, 6, game.rev_food, 1);
        draw_text(1, 8, "Ore :", 1);
        draw_number(15, 8, game.rev_ore, 1);
        draw_text(1, 10, "Farming :", 1);
        draw_number(15, 10, game.rev_culture, 1);
    } else {
        draw_text(4, 1, "Revenus 1/2", 1);
        draw_text(1, 4, "Loyers :", 1);
        draw_number(15, 4, game.rev_rents, 1);
        draw_text(1, 6, "Nourriture :", 1);
        draw_number(15, 6, game.rev_food, 1);
        draw_text(1, 8, "Minerai :", 1);
        draw_number(15, 8, game.rev_ore, 1);
        draw_text(1, 10, "Cultures :", 1);
        draw_number(15, 10, game.rev_culture, 1);
    }
}

// --- PAGE 3 : REVENUS 2/2 ---
void stats_draw_page3(void) {
    clear_inner();
    if (game.language == LANG_EN) {
        draw_text(4, 1, "Income 2/2", 1);
        draw_text(1, 4, "Bar :", 1);
        draw_number(15, 4, game.rev_bar, 1);
        draw_text(1, 6, "Mall :", 1);
        draw_number(15, 6, game.rev_mall, 1);
        draw_text(1, 8, "Cannery :", 1);
        draw_number(15, 8, game.rev_wood, 1);
    } else {
        draw_text(4, 1, "Revenus 2/2", 1);
        draw_text(1, 4, "Bar :", 1);
        draw_number(15, 4, game.rev_bar, 1);
        draw_text(1, 6, "Magasin :", 1);
        draw_number(15, 6, game.rev_mall, 1);
        draw_text(1, 8, "Conserverie :", 1);
        draw_number(15, 8, game.rev_wood, 1);
    }
}

// --- PAGE 4 : GOALS/OBJECTIFS ---
void stats_draw_page4(void) {
    clear_inner();
    restore_shop_tiles();
    if (game.language == LANG_EN) {
        draw_text(8, 1, "Goals", 1);
    } else {
        draw_text(6, 1, "Objectifs", 1);
    }

    if (game.game_mode == MODE_STORY && game.mission_id >= 4) {
        if (game.language == LANG_EN) {
            draw_text(3, 4, "All goals", 1);
            draw_text(3, 6, "have been", 1);
            draw_text(3, 8, "achieved", 1);
        } else {
            draw_text(3, 4, "Toutes les", 1);
            draw_text(3, 6, "missions", 1);
            draw_text(3, 8, "accomplies", 1);
        }
    } else if (game.game_mode == MODE_STORY) {
        MissionStep cur;
        nb_story_get_current_step_b2(&cur);
        uint8_t row = 7;
        uint8_t chk_spr = 1u;
        if (cur.target_money > 0) {
            if (game.money >= cur.target_money) set_sprite_tile(chk_spr, TILE_HUD_TICK);
            else                                set_sprite_tile(chk_spr, 0);
            set_sprite_prop(chk_spr, 0x00u);
            move_sprite(chk_spr, 16u, (uint8_t)(row * 8u + 16u));
            chk_spr++;
            draw_text(3, row, "Argent:", 1);
            draw_number(13, row, cur.target_money, 1);
            row += 2;
        }
        if (cur.target_pop > 0) {
            if (game.population >= cur.target_pop) set_sprite_tile(chk_spr, TILE_HUD_TICK);
            else                                    set_sprite_tile(chk_spr, 0);
            set_sprite_prop(chk_spr, 0x00u);
            move_sprite(chk_spr, 16u, (uint8_t)(row * 8u + 16u));
            chk_spr++;
            draw_text(3, row, "Pop:", 1);
            draw_number(13, row, cur.target_pop, 1);
            row += 2;
        }
        if (cur.target_food_stock > 0) {
            if (game.foodStock >= cur.target_food_stock) set_sprite_tile(chk_spr, TILE_HUD_TICK);
            else                                          set_sprite_tile(chk_spr, 0);
            set_sprite_prop(chk_spr, 0x00u);
            move_sprite(chk_spr, 16u, (uint8_t)(row * 8u + 16u));
            chk_spr++;
            draw_text(3, row, "R~serves:", 1);
            draw_number(14, row, cur.target_food_stock, 1);
            row += 2;
        }
        if (cur.target_food_prod > 0) {
            if (game.foodProduction >= cur.target_food_prod) set_sprite_tile(chk_spr, TILE_HUD_TICK);
            else                                               set_sprite_tile(chk_spr, 0);
            set_sprite_prop(chk_spr, 0x00u);
            move_sprite(chk_spr, 16u, (uint8_t)(row * 8u + 16u));
            chk_spr++;
            draw_text(3, row, "Prod.food:", 1);
            draw_number(15, row, cur.target_food_prod, 1);
            row += 2;
        }
        if (cur.target_happiness > 0) {
            if (game.avg_happiness >= cur.target_happiness) set_sprite_tile(chk_spr, TILE_HUD_TICK);
            else                                              set_sprite_tile(chk_spr, 0);
            set_sprite_prop(chk_spr, 0x00u);
            move_sprite(chk_spr, 16u, (uint8_t)(row * 8u + 16u));
            chk_spr++;
            draw_text(3, row, "Bonheur:", 1);
            draw_number(14, row, cur.target_happiness, 1);
            row += 2;
        }
        if (cur.target_ore > 0) {
            if (game.ore_stock >= cur.target_ore) set_sprite_tile(chk_spr, TILE_HUD_TICK);
            else                                   set_sprite_tile(chk_spr, 0);
            move_sprite(chk_spr, 16u, (uint8_t)(row * 8u + 16u));
            chk_spr++;
            draw_text(3, row, "Minerai:", 1);
            draw_number(14, row, cur.target_ore, 1);
            row += 2;
        }
        if (cur.target_culture > 0) {
            if (game.culture_stock >= cur.target_culture) set_sprite_tile(chk_spr, TILE_HUD_TICK);
            else                                           set_sprite_tile(chk_spr, 0);
            set_sprite_prop(chk_spr, 0x00u);
            move_sprite(chk_spr, 16u, (uint8_t)(row * 8u + 16u));
            chk_spr++;
            draw_text(5, row, "Culture:", 1);
            draw_number(14, row, cur.target_culture, 1);
            row += 2;
        }
        if (cur.target_type != 0) {
            uint8_t bldg_cnt = 0;
            uint8_t ri;
            for (ri = 0; ri < building_count; ri++)
                if (building_registry[ri].type == cur.target_type) bldg_cnt++;
            if (bldg_cnt >= cur.target_count) set_sprite_tile(chk_spr, TILE_HUD_TICK);
            else                               set_sprite_tile(chk_spr, 0);
            set_sprite_prop(chk_spr, 0x00u);
            move_sprite(chk_spr, 16u, (uint8_t)(row * 8u + 16u));
            chk_spr++;
            if      (cur.target_type == TILE_FARM_NW)       draw_text(3, row, "Fermes:", 1);
            else if (cur.target_type == TILE_HOUSE_NW)      draw_text(3, row, "Maisons:", 1);
            else if (cur.target_type == TILE_MINE_NW)       draw_text(3, row, "Mines:", 1);
            else if (cur.target_type == TILE_PLANTATION_NW) draw_text(3, row, "Plantation:", 1);
            else if (cur.target_type == TILE_CHURCH_NW)     draw_text(3, row, "Eglise:", 1);
            else if (cur.target_type == TILE_SCHOOL_NW)     draw_text(3, row, "Ecole:", 1);
            else if (cur.target_type == TILE_HOSPITAL_NW)   draw_text(3, row, "Hopital:", 1);
            else if (cur.target_type == TILE_POLICE_NW)     draw_text(3, row, "Police:", 1);
            else if (cur.target_type == TILE_POWER_NW)      draw_text(3, row, "Centrale:", 1);
            else                                             draw_text(3, row, "Batiment:", 1);
            draw_number(15, row, cur.target_count, 1);
            row += 2;
        }
        if (cur.target_type2 != 0) {
            uint8_t bldg_cnt = 0;
            uint8_t ri;
            for (ri = 0; ri < building_count; ri++)
                if (building_registry[ri].type == cur.target_type2) bldg_cnt++;
            if (bldg_cnt >= cur.target_count2) set_sprite_tile(chk_spr, TILE_HUD_TICK);
            else                                set_sprite_tile(chk_spr, 0);
            set_sprite_prop(chk_spr, 0x00u);
            move_sprite(chk_spr, 16u, (uint8_t)(row * 8u + 16u));
            chk_spr++;
            if      (cur.target_type2 == TILE_FARM_NW)       draw_text(3, row, "Fermes:", 1);
            else if (cur.target_type2 == TILE_HOUSE_NW)      draw_text(3, row, "Maisons:", 1);
            else if (cur.target_type2 == TILE_MINE_NW)       draw_text(3, row, "Mines:", 1);
            else if (cur.target_type2 == TILE_PLANTATION_NW) draw_text(3, row, "Plantation:", 1);
            else if (cur.target_type2 == TILE_CHURCH_NW)     draw_text(3, row, "Eglise:", 1);
            else if (cur.target_type2 == TILE_SCHOOL_NW)     draw_text(3, row, "Ecole:", 1);
            else if (cur.target_type2 == TILE_HOSPITAL_NW)   draw_text(3, row, "Hopital:", 1);
            else if (cur.target_type2 == TILE_POLICE_NW)     draw_text(3, row, "Police:", 1);
            else if (cur.target_type2 == TILE_POWER_NW)      draw_text(3, row, "Centrale:", 1);
            else                                              draw_text(3, row, "Batiment:", 1);
            draw_number(15, row, cur.target_count2, 1);
            row += 2;
        }
    } else {
        if (game.language == LANG_EN) draw_text(1, 6, "Free play", 1);
        else                          draw_text(1, 6, "Partie libre", 1);
    }
}

// --- PAGE 5 : ELECTRICITE ---
void stats_draw_page5(void) {
    clear_inner();
    if (game.language == LANG_EN) {
        draw_text(5, 1, "Electricity", 1);
        draw_text(1, 5, "Produced :", 1);
        draw_number(15, 5, game.electricity_prod, 1);
        draw_text(1, 7, "Consumed :", 1);
        draw_number(15, 7, game.electricity_cons, 1);
    } else {
        draw_text(5, 1, "Electricit~", 1);
        draw_text(1, 5, "Produite :", 1);
        draw_number(15, 5, game.electricity_prod, 1);
        draw_text(1, 7, "Consomm~e :", 1);
        draw_number(15, 7, game.electricity_cons, 1);
    }
}

// --- PAGE 6 : DEPENSES ---
void stats_draw_page6(void) {
    clear_inner();
    if (game.language == LANG_EN) {
        draw_text(5, 1, "Expenses", 1);
        draw_text(1, 5, "Maintenance :", 1);
        draw_number(15, 5, game.exp_maintenance, 1);
        draw_text(1, 7, "Wages :", 1);
        draw_number(15, 7, game.exp_salaries, 1);
        draw_text(1, 9, "Building :", 1);
        draw_number(15, 9, game.exp_construction, 1);
        if (game.decree_tram) {
            draw_text(1, 11, "Tram :", 1);
            draw_number(15, 11, 1000, 1);
            draw_text(1, 13, "Total :", 1);
            draw_number(15, 13, (uint16_t)game.monthly_expenses, 1);
        } else {
            draw_text(1, 11, "Total :", 1);
            draw_number(15, 11, (uint16_t)game.monthly_expenses, 1);
        }
    } else {
        draw_text(5, 1, "D~penses", 1);
        draw_text(1, 5, "Maintenance :", 1);
        draw_number(15, 5, game.exp_maintenance, 1);
        draw_text(1, 7, "Salaires :", 1);
        draw_number(15, 7, game.exp_salaries, 1);
        draw_text(1, 9, "Construction :", 1);
        draw_number(15, 9, game.exp_construction, 1);
        if (game.decree_tram) {
            draw_text(1, 11, "Tram :", 1);
            draw_number(15, 11, 1000, 1);
            draw_text(1, 13, "Total :", 1);
            draw_number(15, 13, (uint16_t)game.monthly_expenses, 1);
        } else {
            draw_text(1, 11, "Total :", 1);
            draw_number(15, 11, (uint16_t)game.monthly_expenses, 1);
        }
    }
}

// --- PAGE 7 : FACTEURS BONHEUR 1/2 ---
void stats_draw_page7(void) {
    clear_inner();
    draw_text(2, 1, "Facteurs bonheur", 1);
    draw_text(6, 2, "1/2", 1);
    {
        int16_t bldg_hap = game.hap_buildings;
        draw_text(1, 4, "Batiments :", 1);
        if (bldg_hap >= 0) { draw_text(13, 4, "+", 1); draw_number(14, 4, (uint16_t)bldg_hap, 1); }
        else               { draw_text(13, 4, "-", 1); draw_number(14, 4, (uint16_t)(-bldg_hap), 1); }
    }
    {
        int16_t v = -(int16_t)(game.crime_rate / 5);
        draw_text(1, 6, "Crime :", 1);
        if (v >= 0) { draw_text(13, 6, "+", 1); draw_number(14, 6, (uint16_t)v, 1); }
        else        { draw_text(13, 6, "-", 1); draw_number(14, 6, (uint16_t)(-v), 1); }
    }
    {
        int16_t v = -(int16_t)(game.unemployment_rate / 5);
        draw_text(1, 8, "Chomage :", 1);
        if (v >= 0) { draw_text(13, 8, "+", 1); draw_number(14, 8, (uint16_t)v, 1); }
        else        { draw_text(13, 8, "-", 1); draw_number(14, 8, (uint16_t)(-v), 1); }
    }
    {
        int16_t pen = (int16_t)(game.homeless * 2);
        if (pen > 20) pen = 20;
        pen = -pen;
        draw_text(1, 10, "Sans-abri :", 1);
        if (pen >= 0) { draw_text(13, 10, "+", 1); draw_number(14, 10, (uint16_t)pen, 1); }
        else          { draw_text(13, 10, "-", 1); draw_number(14, 10, (uint16_t)(-pen), 1); }
    }
    {
        int16_t v = (game.electricity_prod < game.electricity_cons) ? -10 : 0;
        draw_text(1, 12, "Electricit~ :", 1);
        if (v >= 0) { draw_text(13, 12, "+", 1); draw_number(14, 12, (uint16_t)v, 1); }
        else        { draw_text(13, 12, "-", 1); draw_number(14, 12, (uint16_t)(-v), 1); }
    }
}

// --- PAGE 8 : FACTEURS BONHEUR 2/2 ---
void stats_draw_page8(void) {
    clear_inner();
    draw_text(2, 1, "Facteurs bonheur", 1);
    draw_text(6, 2, "2/2", 1);
    {
        int8_t v = game.health_hap_bonus;
        draw_text(1, 4, "Sant~ :", 1);
        if (v >= 0) { draw_text(13, 4, "+", 1); draw_number(14, 4, (uint16_t)v, 1); }
        else        { draw_text(13, 4, "-", 1); draw_number(14, 4, (uint16_t)(-v), 1); }
    }
    {
        int8_t v = game.decree_food_hap_bonus;
        draw_text(1, 6, "D~cret food :", 1);
        if (v >= 0) { draw_text(13, 6, "+", 1); draw_number(14, 6, (uint16_t)v, 1); }
        else        { draw_text(13, 6, "-", 1); draw_number(14, 6, (uint16_t)(-v), 1); }
    }
    {
        draw_text(1, 8, "Famine :", 1);
        if (game.is_in_famine) { draw_text(13, 8, "-", 1); draw_number(14, 8, 1, 1); }
        else                   { draw_text(13, 8, "0", 1); }
    }
    draw_text(1, 11, "Bonheur total :", 1);
    draw_number(15, 12, game.avg_happiness, 1);
}
