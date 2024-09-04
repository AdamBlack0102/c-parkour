//
// Created by goat2 on 2024/1/20.
//

#ifndef FINAL_PROJECT_DISPLAY_H
#define FINAL_PROJECT_DISPLAY_H

#include "../include/common.h"

#define CHARACTER_DELAY 32
#define SCROLL_DELAY 16

SDL_Texture *load_img(char *file);

void do_menu();

void display_scores();
void display_status(bool reversed, bool invincible);
void display_game_over_screen();

void scroll(double scroll, SDL_Texture *texture, SDL_Texture *texture_2);

void present();

void init_obstacles();
void generate_obstacle(SDL_Texture *bird, SDL_Texture *hole, SDL_Texture *stone);
void update_obstacles(double scroll, SDL_Texture *bird, SDL_Texture *hole, SDL_Texture *stone);

void init_items();
void generate_item(SDL_Texture *mushroom, SDL_Texture *star, SDL_Texture *coin);
void update_item(double scroll, SDL_Texture *mushroom, SDL_Texture *star, SDL_Texture *coin);

void destroy_texture(SDL_Texture* texture);

void blit(SDL_Texture *texture, int x, int y);

#endif //FINAL_PROJECT_DISPLAY_H
