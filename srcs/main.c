#define SDL_MAIN_HANDLED

#include "../include/main.h"

enum GameStatus game_status = MENU;

SDL_Event event;

bool reverse = false;
bool quit = false;
bool pause = false;
bool game_over = false;

int scr = 0;
int count = 0;
int count_obstacle = 0;
int count_item = 0;
int count_invincible = 0;
int count_reverse = 0;

double speed = 1.0;
double score = 0.0;


int main(int argc, char *argv[]) {
    init_app();
    init_character();
    init_keyboard();

    SDL_Texture *texture_background = load_img("./pic/path.png");
    SDL_Texture *texture_background_2 = load_img("./pic/path.png");
    SDL_Texture *texture_character_still = load_img("./pic/back_still.png");
    SDL_Texture *texture_character_left = load_img("./pic/back_left.png");
    SDL_Texture *texture_character_right = load_img("./pic/back_right.png");
    SDL_Texture *texture_character_slide = load_img("./pic/back_slide.png");
    SDL_Texture *texture_hole = load_img("./pic/hole.png");
    SDL_Texture *texture_bird = load_img("./pic/bird.png");
    SDL_Texture *texture_stone = load_img("./pic/stone.png");
    SDL_Texture *texture_star = load_img("./pic/star.png");
    SDL_Texture *texture_mushroom = load_img("./pic/mushroom.png");
    SDL_Texture *texture_coin = load_img("./pic/coin.png");

    player.texture = texture_character_still;
    SDL_RenderCopy(app.renderer, texture_background, NULL, NULL);
    SDL_QueryTexture(player.texture, NULL, NULL, &player.w, &player.h);

    srand(time(NULL));

    init_obstacles();
    init_items();

    if (game_status == MENU) {
        do_menu(event);
    }

    while (!quit) {
        if (!game_over) {
            CollisionBox player_collision_box = {player.x, player.y, player.w, player.h};

            character_move(texture_character_still, texture_character_slide);

            if (pause) {
                continue;
            }

            player_collision_box.x = player.x;
            player_collision_box.y = player.y;

            init_hit();

            check_obstacle_hit(player_collision_box);
            check_item_hit(player_collision_box);

            if (speed <= 10 && ((int) score % 10 == 0)) {
                speed += 0.01;
            }

            if (player.is_invincible) {
                do_invincible();
            } else {
                do_normal();
            }

            SDL_RenderClear(app.renderer);

            if (game_status == GAME) {
                scr++;
                if (scr >= SCREEN_H) {
                    scr = 0;
                }

                scroll(scr, texture_background, texture_background_2);

                for (int i = 0; i < MAX_OBSTACLES; i++) {
                    if (obstacles[i].texture != texture_background &&
                        obstacles[i].texture != texture_background_2) {
                        update_obstacles(speed, texture_bird, texture_hole, texture_stone);
                        break;
                    }
                }

                if (count_obstacle % GENERATE_OBSTACLE_DELAY == 0) {
                    generate_obstacle(texture_bird, texture_hole, texture_stone);
                    count_obstacle++;
                }
                count_obstacle++;

                update_item(speed, texture_mushroom, texture_star, texture_coin);

                if (count_item % GENERATE_ITEMS_DELAY == 0) {
                    generate_item(texture_mushroom, texture_star, texture_coin);
                    count_item++;
                }
                count_item++;

                if (count % CHARACTER_DELAY == 0) {
                    if (player.texture == texture_character_still ||
                        player.texture == texture_character_right) {
                        player.texture = texture_character_left;
                    } else if (player.texture == texture_character_left) {
                        player.texture = texture_character_right;
                    }
                }
                count++;
                blit(player.texture, player.x, player.y);

                display_status(reverse, player.is_invincible);

                display_scores();

                present();

                SDL_Delay(SCROLL_DELAY);

            }
        } else if (game_over) {
            display_game_over_screen();
            present();
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    pause = false;
                    quit = true;
                }
                if (event.type == SDL_KEYDOWN) {
                    if (event.key.keysym.scancode == SDL_SCANCODE_R) {
                        restart_game();
                    }
                }
            }
        }
    }

    destroy_texture(player.texture);
    destroy_texture(texture_background);
    destroy_texture(texture_character_still);
    destroy_texture(texture_character_left);
    destroy_texture(texture_character_right);
    destroy_texture(texture_character_slide);
    destroy_texture(texture_background);
    destroy_texture(texture_background_2);
    destroy_texture(texture_coin);
    destroy_texture(texture_star);
    destroy_texture(texture_mushroom);
    destroy_texture(texture_stone);
    destroy_texture(texture_hole);
    destroy_texture(texture_bird);

    quit_keyboard();

    SDL_DestroyWindow(app.window);
    quit_renderer();

    quit_app();

    return 0;
}

void init_app() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        HANDLE_ERROR("Init SDL");
    }

    app.window = SDL_CreateWindow("A Humble Pixel Parkour", 500, 150, SCREEN_W, SCREEN_H, SDL_WINDOW_SHOWN);
    app.renderer = SDL_CreateRenderer(app.window, -1, SDL_RENDERER_ACCELERATED);

    if (!IMG_Init(IMG_INIT_PNG)) {
        HANDLE_ERROR("Init Image");
    }

    if (TTF_Init() == -1) {
        HANDLE_ERROR("Init TTF");
    }
}

void quit_app() {
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void quit_renderer() {
    SDL_DestroyRenderer(app.renderer);
}

void init_keyboard() {
    app.keyboard = calloc(SDL_NUM_SCANCODES, sizeof(bool));
}

void quit_keyboard() {
    free(app.keyboard);
}

void do_keydown(SDL_KeyboardEvent *key,
                SDL_Texture *texture_character_still) {
    app.keyboard[key->keysym.scancode] = true;

    if ((key->keysym.scancode == SDL_SCANCODE_LEFT ||
         key->keysym.scancode == SDL_SCANCODE_A)
        && (player.x > 150)) {
        player.x -= 100;
    } else if ((key->keysym.scancode == SDL_SCANCODE_RIGHT ||
                key->keysym.scancode == SDL_SCANCODE_D)
               && (player.x < 200)) {
        player.x += 100;
    } else if (key->keysym.scancode == SDL_SCANCODE_UP ||
               key->keysym.scancode == SDL_SCANCODE_SPACE ||
               key->keysym.scancode == SDL_SCANCODE_W) {
        player.texture = texture_character_still;
        player.is_jumping = true;
        player.is_running = false;
        player.y = 470;
    } else if (key->keysym.scancode == SDL_SCANCODE_DOWN ||
               key->keysym.scancode == SDL_SCANCODE_S) {
        player.texture = texture_character_still;
        player.is_running = false;
        player.is_sliding = true;
        player.y = 470;
    } else if (key->keysym.scancode == SDL_SCANCODE_R) {
        restart_game();
    }
}

void do_keydown_reverse(SDL_KeyboardEvent *key,
                        SDL_Texture *texture_character_still) {
    app.keyboard[key->keysym.scancode] = true;

    if ((key->keysym.scancode == SDL_SCANCODE_LEFT ||
         key->keysym.scancode == SDL_SCANCODE_A)
        && (player.x < 200)) {
        player.x += 100;
    } else if ((key->keysym.scancode == SDL_SCANCODE_RIGHT ||
                key->keysym.scancode == SDL_SCANCODE_D)
               && (player.x > 150)) {
        player.x -= 100;
    } else if (key->keysym.scancode == SDL_SCANCODE_UP ||
               key->keysym.scancode == SDL_SCANCODE_SPACE ||
               key->keysym.scancode == SDL_SCANCODE_W) {
        player.texture = texture_character_still;
        player.is_running = false;
        player.is_sliding = true;
        player.y = 470;
    } else if (key->keysym.scancode == SDL_SCANCODE_DOWN ||
               key->keysym.scancode == SDL_SCANCODE_S) {
        player.texture = texture_character_still;
        player.is_jumping = true;
        player.is_running = false;
        player.y = 470;
    } else if (key->keysym.scancode == SDL_SCANCODE_R) {
        restart_game();
    }
}

void do_keyup(SDL_KeyboardEvent *key,
              SDL_Texture *texture_character_still) {
    app.keyboard[key->keysym.scancode] = false;
    if ((key->keysym.scancode == SDL_SCANCODE_LEFT) ||
        (key->keysym.scancode == SDL_SCANCODE_A) ||
        (key->keysym.scancode == SDL_SCANCODE_RIGHT) ||
        (key->keysym.scancode == SDL_SCANCODE_D)) {
        player.is_running = true;
        player.is_jumping = false;
        player.is_sliding = false;
        player.y = 470;
    } else if (key->keysym.scancode == SDL_SCANCODE_UP ||
               key->keysym.scancode == SDL_SCANCODE_W) {
        player.is_jumping = false;
        player.is_running = true;
        player.is_sliding = false;
        player.texture = texture_character_still;
    } else if (key->keysym.scancode == SDL_SCANCODE_DOWN ||
               key->keysym.scancode == SDL_SCANCODE_S) {
        player.is_sliding = false;
        player.is_running = true;
        player.is_jumping = false;
        player.texture = texture_character_still;
    } else if (key->keysym.scancode == SDL_SCANCODE_R) {
        restart_game();
    }
}

void init_character() {
    player.is_running = true;
    player.is_jumping = false;
    player.is_sliding = false;
    player.is_invincible = false;

    player.x = CHARACTER_INIT_X;
    player.y = CHARACTER_INIT_Y;
}



