//
// Created by goat2 on 2024/1/20.
//

#include "../include/main.h"
#include "../include/display.h"
#include "../include/game.h"
#include "../include/common.h"

int item_count = 0;
int obstacle_count = 0;

bool check_collision(CollisionBox box, int x2, int y2) {
    if (x2 + 10 >= box.x && x2 - 10 <= box.x + box.w &&
        y2 - 10 >= box.y && y2 + 10 <= box.y + box.h) {
        return true;
    }
    return false;
}

bool overlap(int random_position) {
    for (int i = 0; i < obstacle_count; i++) {
        if (obstacles[i].texture == NULL) {
            continue;
        }

        if (obstacles[i].x == 70 + 100 * random_position) {
            if (abs(obstacles[i].y - obstacles[i].h) < MIN_DISTANCE) {
                return true;
            }

            for (int j = 0; j < obstacle_count; j++) {
                if (j != i && obstacles[j].texture != NULL
                    && obstacles[j].x == 70 + 100 * random_position
                    && abs(obstacles[i].y - obstacles[j].y) < MIN_DISTANCE) {
                    return true;
                }
            }

            for (int y = 0; y < item_count; y++) {
                if (items[y].texture != NULL
                    && items[y].x == 70 + 100 * random_position
                    && abs(obstacles[i].y - items[y].y) < MIN_DISTANCE) {
                    return true;
                }
            }
        }
    }

    for (int i = 0; i < item_count; i++) {
        if (items[i].texture == NULL) {
            continue;
        }

        if (items[i].x == 70 + 100 * random_position) {
            if (abs(items[i].y - items[i].h) < MIN_DISTANCE) {
                return true;
            }

            for (int j = 0; j < obstacle_count; j++) {
                if (obstacles[j].texture != NULL
                    && obstacles[j].x == 70 + 100 * random_position
                    && abs(items[i].y - obstacles[j].y) < MIN_DISTANCE) {
                    return true;
                }
            }

            for (int j = 0; j < item_count; j++) {
                if (j != i && items[j].texture != NULL
                    && items[j].x == 70 + 100 * random_position
                    && abs(items[i].y - items[j].y) < MIN_DISTANCE) {
                    return true;
                }
            }
        }
    }

    return false;
}

void generate_bird(int random_position, SDL_Texture *bird) {
    if (!overlap(random_position)) {
        if (obstacle_count < MAX_OBSTACLES) {
            obstacle_count++;
        } else {
            obstacle_count = 0;
        }
        if (obstacles[obstacle_count].texture == NULL) {
            SDL_QueryTexture(bird, NULL, NULL, NULL, &obstacles[obstacle_count].h);
            obstacles[obstacle_count].x = 70 + 100 * random_position;
            obstacles[obstacle_count].y = -obstacles[obstacle_count].h - 80;
            obstacles[obstacle_count].texture = bird;
            obstacles[obstacle_count].type = 0;
        }
    }
}

void generate_hole(int random_position, SDL_Texture *hole) {
    if (!overlap(random_position)) {
        if (obstacle_count < MAX_OBSTACLES) {
            obstacle_count++;
        } else {
            obstacle_count = 0;
        }
        if (obstacles[obstacle_count].texture == NULL) {
            SDL_QueryTexture(hole, NULL, NULL, NULL, &obstacles[obstacle_count].h);
            obstacles[obstacle_count].x = 70 + 100 * random_position;
            obstacles[obstacle_count].y = -obstacles[obstacle_count].h - 80;
            obstacles[obstacle_count].texture = hole;
            obstacles[obstacle_count].type = 1;
        }
    }
}

void generate_stone(int random_position, SDL_Texture *stone) {
    if (!overlap(random_position)) {
        if (obstacle_count < MAX_OBSTACLES) {
            obstacle_count++;
        } else {
            obstacle_count = 0;
        }
        if (obstacles[obstacle_count].texture == NULL) {
            SDL_QueryTexture(stone, NULL, NULL, NULL, &obstacles[obstacle_count].h);
            obstacles[obstacle_count].x = 70 + 100 * random_position;
            obstacles[obstacle_count].y = -obstacles[obstacle_count].h - 80;
            obstacles[obstacle_count].texture = stone;
            obstacles[obstacle_count].type = 2;
        }
    }
}

void generate_mushroom(int random_position, SDL_Texture *mushroom) {
    if (!overlap(random_position)) {
        if (item_count < MAX_ITEMS) {
            item_count++;
        } else {
            item_count = 0;
        }
        if (items[item_count].texture == NULL) {
            SDL_QueryTexture(mushroom, NULL, NULL, NULL, &items[item_count].h);
            items[item_count].x = 70 + 100 * random_position;
            items[item_count].y = -items[item_count].h - 190;
            items[item_count].texture = mushroom;
            items[item_count].type = 0;
        }
    }
}

void generate_star(int random_position, SDL_Texture *star) {
    if (!overlap(random_position)) {
        if (item_count < MAX_ITEMS) {
            item_count++;
        } else {
            item_count = 0;
        }
        if (items[item_count].texture == NULL) {
            SDL_QueryTexture(star, NULL, NULL, NULL, &items[item_count].h);
            items[item_count].x = 70 + 100 * random_position;
            items[item_count].y = -items[item_count].h - 190;
            items[item_count].texture = star;
            items[item_count].type = 1;
        }
    }
}

void generate_coin(int random_position, SDL_Texture *coin) {
    if (!overlap(random_position)) {
        if (item_count < MAX_ITEMS) {
            item_count++;
        } else {
            item_count = 0;
        }
        if (items[item_count].texture == NULL) {
            SDL_QueryTexture(coin, NULL, NULL, NULL, &items[item_count].h);
            items[item_count].x = 70 + 100 * random_position;
            items[item_count].y = -items[item_count].h - 130;
            items[item_count].texture = coin;
            items[item_count].type = 2;
        }
    }
}

void do_invincible() {
    reverse = false;
    if (count_invincible < 600) {
        count_invincible++;
    } else {
        player.is_invincible = false;
        count_invincible = 0;
    }
}

void do_normal() {
    if (hit_mushroom) {
        reverse = true;
    }

    if (reverse) {
        if (count_reverse < 600) {
            count_reverse++;
        } else {
            reverse = false;
            count_reverse = 0;
        }
    }

    if (player.is_jumping) {
        if (hit_bird || hit_stone) {
            game_over = true;
        }
    } else if (player.is_sliding) {
        if (hit_hole || hit_stone) {
            game_over = true;
        }
    } else if (player.is_running) {
        if (hit_hole || hit_stone || hit_bird) {
            game_over = true;
        }
    }
}

void character_move(SDL_Texture *texture_character_still,
                    SDL_Texture *texture_character_slide) {
    while (SDL_PollEvent(&event) != 0) {
        if (event.type == SDL_QUIT) {
            quit = true;
        } else if (event.type == SDL_KEYDOWN) {
            if (!reverse) {
                do_keydown(&event.key, texture_character_still);
            } else {
                do_keydown_reverse(&event.key, texture_character_still);
            }
            if (player.is_jumping) {
                player.y = JUMP_HEIGHT;
            } else if (player.is_running) {
                player.y = CHARACTER_INIT_Y;
            } else if (player.is_sliding) {
                player.texture = texture_character_slide;
            }
            if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                if (!pause) {
                    pause = true;
                } else {
                    pause = false;
                }
            }
        } else if (event.type == SDL_KEYUP) {
            do_keyup(&event.key, texture_character_still);
            player.y = CHARACTER_INIT_Y;
        }
    }
}


void init_hit() {
    hit_bird = false;
    hit_hole = false;
    hit_stone = false;
    hit_mushroom = false;
    hit_star = false;
    hit_coin = false;
}

void check_item_hit(CollisionBox player_collision_box) {
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (items[i].texture != NULL) {
            if (items[i].type == 0) {
                hit_mushroom = check_collision(player_collision_box, items[i].x, items[i].y);
                break;
            } else if (items[i].type == 1) {
                hit_star = check_collision(player_collision_box, items[i].x, items[i].y);
                break;
            } else if (items[i].type == 2) {
                hit_coin = check_collision(player_collision_box,
                                           items[i].x + COIN_OFFSET_X, items[i].y - COIN_OFFSET_Y);
                break;
            }
        }
    }

    if (hit_star) {
        player.is_invincible = true;
    }
    if (hit_coin) {
        score += 0.5;
    }

    score += 0.01;
}

void check_obstacle_hit(CollisionBox player_collision_box) {
    for (int i = 0; i < MAX_OBSTACLES; i++) {
        if (obstacles[i].texture != NULL) {
            if (obstacles[i].type == 0) {
                hit_bird = check_collision(player_collision_box, obstacles[i].x,
                                           obstacles[i].y + OBSTACLE_OFFSET);
                break;
            } else if (obstacles[i].type == 1) {
                hit_hole = check_collision(player_collision_box, obstacles[i].x,
                                           obstacles[i].y);
                break;
            } else if (obstacles[i].type == 2) {
                hit_stone = check_collision(player_collision_box, obstacles[i].x,
                                            obstacles[i].y + OBSTACLE_OFFSET);
                break;
            }
        }
    }
}

void restart_app() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        HANDLE_ERROR("Init SDL");
    }

    if (!IMG_Init(IMG_INIT_PNG)) {
        HANDLE_ERROR("Init Image");
    }

    if (TTF_Init() == -1) {
        HANDLE_ERROR("Init TTF");
    }
}

void restart_game() {
    restart_app();
    init_character();
    init_keyboard();

    scr = 0;
    count = 0;
    count_obstacle = 0;
    count_item = 0;
    count_invincible = 0;
    count_reverse = 0;

    speed = 1.0;
    score = 0.0;

    reverse = false;
    quit = false;
    pause = false;
    game_over = false;

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
        do_menu();
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
}
