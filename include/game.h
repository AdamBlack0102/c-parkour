//
// Created by goat2 on 2024/1/20.
//

#ifndef FINAL_PROJECT_GAME_H
#define FINAL_PROJECT_GAME_H

#include "../include/main.h"

#define CHARACTER_INIT_X 175
#define CHARACTER_INIT_Y 470

typedef struct {
    SDL_Texture *texture;
    int x, y, w, h;
    bool is_jumping;
    bool is_sliding;
    bool is_running;
    bool is_invincible;
} Character;

typedef struct {
    int x;
    int y;
    int w;
    int h;
} CollisionBox;

Character player;

bool check_collision(CollisionBox box1, int x2, int y2);

bool overlap(int random_position);

void init_hit();

void restart_app();
void restart_game();

void generate_bird(int random_position, SDL_Texture *bird);
void generate_hole(int random_position, SDL_Texture *hole);
void generate_stone(int random_position, SDL_Texture *stone);

void generate_mushroom(int random_position, SDL_Texture *mushroom);
void generate_star(int random_position, SDL_Texture *star);
void generate_coin(int random_position, SDL_Texture *coin);

void do_invincible();
void do_normal();

void character_move(SDL_Texture *texture_character_still,
                    SDL_Texture *texture_character_slide);

void check_item_hit(CollisionBox player_collision_box);
void check_obstacle_hit(CollisionBox player_collision_box);

#endif //FINAL_PROJECT_GAME_H
