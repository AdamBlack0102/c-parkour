//
// Created by goat2 on 2024/1/20.
//

#ifndef FINAL_PROJECT_COMMON_H
#define FINAL_PROJECT_COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <msplog.h>
#include <time.h>

#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_mixer.h>
#include<SDL2/SDL_ttf.h>

#define HANDLE_ERROR(msg) printf("%s: %s\n", msg, SDL_GetError()); \
  exit(EXIT_FAILURE);

#define SCREEN_W 400
#define SCREEN_H 550

#define MIN_DISTANCE 90
#define MAX_OBSTACLES 9
#define GENERATE_OBSTACLE_DELAY 30

#define MAX_ITEMS 3
#define GENERATE_ITEMS_DELAY 100

#define JUMP_HEIGHT 450

#define OBSTACLE_OFFSET 40
#define COIN_OFFSET_X 15
#define COIN_OFFSET_Y 10

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    bool *keyboard;
    bool jump;
    bool down;
    bool left;
    bool right;
} App;

typedef struct {
    int x;
    double y;
    int type;
    int w, h;
    bool is_in_screen;
    SDL_Texture *texture;
} Obstacles;

typedef struct {
    int x;
    double y;
    int type;
    int w, h;
    bool is_in_screen;
    SDL_Texture *texture;
} Items;

App app;
Obstacles obstacles[MAX_OBSTACLES];
Items items[MAX_ITEMS];

extern SDL_Event event;

extern int last_obstacle_time;
extern int last_item_time;

extern int obstacle_count;
extern int item_count;

extern bool reverse;
extern bool quit;
extern bool pause;
extern bool game_over;

extern int scr;
extern int count;
extern int count_obstacle;
extern int count_item;
extern int count_invincible;
extern int count_reverse;

extern double speed;
extern double score;

#endif //FINAL_PROJECT_COMMON_H
