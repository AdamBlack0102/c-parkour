//
// Created by goat2 on 2024/1/20.
//
#include "../include/main.h"

int last_obstacle_time = 0;
int last_item_time = 0;

void do_menu() {
    SDL_Texture *texture_menu = load_img("./pic/menu.png");
    SDL_RenderCopy(app.renderer, texture_menu, NULL, NULL);
    present();

    while (game_status == MENU) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                game_status = GAME;
                quit = true;
                break;
            }

            if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
                if (event.key.keysym.scancode == SDL_SCANCODE_F) {
                    game_status = GAME;
                }
            }
        }
    }
}

void display_game_over_screen() {
    TTF_Font *font = TTF_OpenFont("./font.ttf", 36);
    if (!font) {
        return;
    }

    SDL_Color color = {32, 102, 35};
    char score_text[20];
    sprintf(score_text, "Score: %.2lf", score);
    SDL_Surface *score_surface = TTF_RenderText_Solid(font, score_text, color);
    if (!score_surface) {
        TTF_CloseFont(font);
        return;
    }

    SDL_Surface *game_over_surface = TTF_RenderText_Solid(font, "Game Over", color);
    if (!game_over_surface) {
        SDL_FreeSurface(score_surface);
        TTF_CloseFont(font);
        return;
    }

    SDL_Texture *score_texture = SDL_CreateTextureFromSurface(app.renderer, score_surface);
    SDL_Texture *game_over_texture = SDL_CreateTextureFromSurface(app.renderer, game_over_surface);

    SDL_Rect score_rect = {100, 300, score_surface->w, score_surface->h};
    SDL_Rect game_over_rect = {120, 200, game_over_surface->w, game_over_surface->h};

    SDL_RenderCopy(app.renderer, score_texture, NULL, &score_rect);
    SDL_RenderCopy(app.renderer, game_over_texture, NULL, &game_over_rect);

    SDL_FreeSurface(score_surface);
    SDL_FreeSurface(game_over_surface);
    SDL_DestroyTexture(score_texture);
    SDL_DestroyTexture(game_over_texture);
    TTF_CloseFont(font);
}

void display_scores() {
    TTF_Font *font = TTF_OpenFont("./font.ttf", 20);
    SDL_Color color = {32, 102, 35};

    SDL_Surface *score_surface = TTF_RenderText_Solid(font, "Score: 0", color);
    if (!score_surface) {
        HANDLE_ERROR("Render text");
    }

    SDL_Texture *score_texture = SDL_CreateTextureFromSurface(app.renderer, score_surface);
    if (!score_texture) {
        HANDLE_ERROR("Create texture");
    }

    SDL_Rect score_rect = {15, 15, score_surface->w, score_surface->h};

    char score_text[20];
    sprintf(score_text, "Score: %.2f", score);

    SDL_FreeSurface(score_surface);
    score_surface = TTF_RenderText_Solid(font, score_text, color);
    if (!score_surface) {
        HANDLE_ERROR("Render text");
    }

    SDL_DestroyTexture(score_texture);
    score_texture = SDL_CreateTextureFromSurface(app.renderer, score_surface);
    if (!score_texture) {
        HANDLE_ERROR("Create texture");
    }

    SDL_RenderCopy(app.renderer, score_texture, NULL, &score_rect);
}

void display_status(bool reversed, bool invincible) {
    TTF_Font *font = TTF_OpenFont("./font.ttf", 18);
    if (!font) {
        HANDLE_ERROR("Open font");
    }

    SDL_Color color = {32, 102, 35};

    char status_text[30];
    if (invincible) {
        sprintf(status_text, "Status: "
                             "invincible");
    } else if (reversed) {
        sprintf(status_text, "Status: "
                             "reversed");
    } else {
        sprintf(status_text, "Status: "
                             "normal");
    }

    SDL_Surface *status_surface = TTF_RenderText_Solid(font, status_text, color);
    if (!status_surface) {
        HANDLE_ERROR("Render text");
    }

    SDL_Texture *status_texture = SDL_CreateTextureFromSurface(app.renderer, status_surface);
    if (!status_texture) {
        HANDLE_ERROR("Create texture");
    }

    SDL_Rect status_rect = {235, 15, status_surface->w, status_surface->h};

    SDL_RenderCopy(app.renderer, status_texture, NULL, &status_rect);

    SDL_FreeSurface(status_surface);
    SDL_DestroyTexture(status_texture);
    TTF_CloseFont(font);
}

void scroll(double scroll, SDL_Texture *texture, SDL_Texture *texture_2) {
    SDL_Rect rect_dest = {0, scroll, SCREEN_W, SCREEN_H};
    SDL_Rect rect_dest_2 = {0, scroll - SCREEN_H, SCREEN_W, SCREEN_H};

    SDL_RenderCopy(app.renderer, texture, NULL, &rect_dest);
    SDL_RenderCopy(app.renderer, texture_2, NULL, &rect_dest_2);
}

SDL_Texture *load_img(char *file) {
    SDL_Texture *img = IMG_LoadTexture(app.renderer, file);
    return img;
}

void blit(SDL_Texture *texture, int x, int y) {
    SDL_Rect dest;

    dest.x = x;
    dest.y = y;
    SDL_QueryTexture(player.texture, NULL, NULL, &dest.w, &dest.h);

    SDL_RenderCopy(app.renderer, texture, NULL, &dest);
}

void init_obstacles() {
    for (int i = 0; i < MAX_OBSTACLES; i++) {
        obstacles[i].texture = NULL;
        obstacles[i].x = 0;
        obstacles[i].y = -200;
        obstacles[i].type = 3;
        obstacles[i].is_in_screen = false;
    }
}

void generate_obstacle(SDL_Texture *bird, SDL_Texture *hole, SDL_Texture *stone) {
    int random_position = rand() % 3;
    int random_type = rand() % 3;

    if (!overlap(random_position)) {
        if (random_type == 0) {
            generate_bird(random_position, bird);
        } else if (random_type == 1) {
            generate_hole(random_position, hole);
        } else if (random_type == 2) {
            generate_stone(random_position, stone);
        }
    }
}

void update_obstacles(double scroll, SDL_Texture *bird, SDL_Texture *hole, SDL_Texture *stone) {

    for (int i = 0; i < MAX_OBSTACLES; i++) {
        obstacles[i].y += scroll;
        blit(obstacles[i].texture, obstacles[i].x, obstacles[i].y);
    }

    for (int i = 0; i < MAX_OBSTACLES; i++) {
        if (obstacles[i].y > SCREEN_H) {
            obstacles[i].texture = NULL;
            int current_time = SDL_GetTicks();
            if (current_time - last_obstacle_time > 100) {
                last_obstacle_time = current_time;

                if (obstacles[i].type == 0) {
                    obstacles[i].texture = bird;
                } else if (obstacles[i].type == 1) {
                    obstacles[i].texture = hole;
                } else if (obstacles[i].type == 2) {
                    obstacles[i].texture = stone;
                }
            }
        }
        blit(obstacles[i].texture, obstacles[i].x, obstacles[i].y);
    }
}

void init_items() {
    for (int i = 0; i < MAX_ITEMS; i++) {
        items[i].texture = NULL;
        items[i].x = 0;
        items[i].y = -200;
        items[i].type = 3;
        items[i].is_in_screen = false;
    }
}

void generate_item(SDL_Texture *mushroom, SDL_Texture *star, SDL_Texture *coin) {
    if (rand() % 3 == 0) {
        int random_position = rand() % 3;
        int random_type = rand() % 100;

        if (random_type > 70 && random_type <= 95) {
            generate_mushroom(random_position, mushroom);
        } else if (random_type > 95) {
            generate_star(random_position, star);
        } else if (random_type < 70) {
            generate_coin(random_position, coin);
        }
    }
}

void update_item(double scroll, SDL_Texture *mushroom, SDL_Texture *star, SDL_Texture *coin) {

    for (int i = 0; i < MAX_ITEMS; i++) {
        items[i].y += scroll;
        blit(items[i].texture, items[i].x, items[i].y);
    }

    for (int i = 0; i < MAX_ITEMS; i++) {
        if (items[i].y > SCREEN_H) {
            items[i].texture = NULL;
            int current_time = SDL_GetTicks();
            if (current_time - last_item_time > 400) {
                generate_item(mushroom, star, coin);
                last_item_time = current_time;
            }
        }
    }
}

void present() {
    SDL_RenderPresent(app.renderer);
}

void destroy_texture(SDL_Texture *texture) {
    if (texture != NULL) {
        SDL_DestroyTexture(texture);
    }
}

