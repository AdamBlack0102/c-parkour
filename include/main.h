//
// Created by goat2 on 2024/1/20.
//

#ifndef FINAL_PROJECT_MAIN_H
#define FINAL_PROJECT_MAIN_H

#include "../include/common.h"
#include "../include/game.h"
#include "../include/display.h"


enum GameStatus{
    MENU, GAME
} ;

extern enum GameStatus game_status;

bool hit_bird;
bool hit_hole;
bool hit_stone;
bool hit_mushroom;
bool hit_star;
bool hit_coin;

void init_app();
void quit_app();

void init_character();

void quit_renderer();

void do_keydown(SDL_KeyboardEvent *key,
                 SDL_Texture *texture_character_still);
void do_keydown_reverse(SDL_KeyboardEvent *key,
                         SDL_Texture *texture_character_still);
void do_keyup(SDL_KeyboardEvent *key,
               SDL_Texture *texture_character_still);

 void init_keyboard();
 void quit_keyboard();

#endif //FINAL_PROJECT_MAIN_H
