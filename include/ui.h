/**
 * CatPong, Multiplayer Pong
 * https://github.com/rohanrhu/CatPong
 * 
 * Copyright (C) 2024, Meowing Cat (https://meowingcat.io) <meowingcate@gmail.com>
 * Licensed under the MIT License.
 */

#pragma once

#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "node.h"
#include "input.h"

#define CATPONG_UI_FONT_PATH "assets/fonts/PixelEmulator-xq08.ttf"

typedef struct catpong_window catpong_window_t;
struct catpong_window {
    catpong_node_t node;
    int width;
    int height;
    SDL_Window *sdl_window;
    SDL_Renderer *sdl_renderer;
    SDL_Color sdl_background_color;
    bool is_running;
};

catpong_window_t *catpong_window_new(const char *title, int x, int y, int width, int height, Uint32 flags);
void catpong_window_free(catpong_window_t *window);
void catpong_window_clear(catpong_window_t *window);
void catpong_window_present(catpong_window_t *window);
void catpong_window_update(catpong_window_t *window);

typedef struct catpong_label catpong_label_t;
struct catpong_label {
    catpong_node_t node;
    catpong_window_t* window;
    char* text;
    SDL_Renderer *sdl_renderer;
    TTF_Font *tff_font;
    SDL_Color sdl_color;
    SDL_Surface *sdl_surface;
    SDL_Texture *sdl_texture;
    SDL_Rect sdl_rect;
    int x;
    int y;
};

catpong_label_t *catpong_label_new(catpong_window_t* window, const char *font_path, int font_size, const char *text, SDL_Color color);
void catpong_label_free(catpong_label_t *label);
void catpong_label_render(catpong_label_t *label, int x, int y);
void catpong_label_update(catpong_label_t *label);
void catpong_label_set_position(catpong_label_t *label, int x, int y);
void catpong_label_set_text(catpong_label_t *label, const char *text);

typedef struct catpong_button catpong_button_t;
struct catpong_button {
    catpong_node_t node;
    catpong_window_t* window;
    char* text;
    SDL_Renderer *sdl_renderer;
    TTF_Font *tff_font;
    SDL_Color sdl_color;
    SDL_Color sdl_background_color;
    SDL_Surface *sdl_surface;
    SDL_Texture *sdl_texture;
    SDL_Rect sdl_rect;
    int x;
    int y;
    int width;
    int height;
    catpong_input_mouse_state_t mouse_state;
    bool is_hovered;
    bool is_hovered_handled;
    bool is_pressed;
    bool is_pressed_handled;
};

catpong_button_t *catpong_button_new(catpong_window_t* window, const char *font_path, int font_size, const char *text, SDL_Color color, SDL_Color background_color);
void catpong_button_free(catpong_button_t *button);
void catpong_button_render(catpong_button_t *button);
void catpong_button_update(catpong_button_t *button);
void catpong_button_set_position(catpong_button_t *button, int x, int y);
void catpong_button_set_size(catpong_button_t *button, int width, int height);
void catpong_button_set_text(catpong_button_t *button, const char *text);
void catpong_button_update_hovered(catpong_button_t *button, int x, int y);
void catpong_button_update_pressed(catpong_button_t *button, bool is_pressed);
bool catpong_button_is_hovered_once(catpong_button_t *button);
bool catpong_button_is_pressed_once(catpong_button_t *button);
void catpong_button_use_mouse_state(catpong_button_t *button, catpong_input_mouse_state_t *mouse_state);