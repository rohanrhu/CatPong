/**
 * CatPong, Multiplayer Pong
 * https://github.com/rohanrhu/CatPong
 * 
 * Copyright (C) 2024, Meowing Cat (https://meowingcat.io) <meowingcate@gmail.com>
 * Licensed under the MIT License.
 */

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "ui.h"

catpong_window_t* catpong_window_new(const char *title, int x, int y, int width, int height, Uint32 flags) {
    catpong_window_t *window = malloc(sizeof(catpong_window_t));
    window->node.type = CATPONG_NODE_TYPE_WINDOW;

    window->width = width;
    window->height = height;

    window->sdl_window = SDL_CreateWindow(title, x, y, width, height, flags);
    window->sdl_renderer = SDL_CreateRenderer(window->sdl_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_SetRenderDrawBlendMode(window->sdl_renderer, SDL_BLENDMODE_BLEND);

    window->is_running = true;

    return window;
}

void catpong_window_free(catpong_window_t *window) {
    SDL_DestroyRenderer(window->sdl_renderer);
    SDL_DestroyWindow(window->sdl_window);
    free(window);
}

void catpong_window_clear(catpong_window_t *window) {
    SDL_SetRenderDrawColor(window->sdl_renderer, 0, 0, 0, 255);
    SDL_RenderClear(window->sdl_renderer);
}

void catpong_window_present(catpong_window_t *window) {
    SDL_RenderPresent(window->sdl_renderer);
}

void catpong_window_update(catpong_window_t *window) {
    catpong_window_present(window);
}

catpong_label_t* catpong_label_new(catpong_window_t* window, const char *font_path, int font_size, const char *text, SDL_Color color) {
    catpong_label_t *label = malloc(sizeof(catpong_label_t));
    label->window = window;
    label->node.type = CATPONG_NODE_TYPE_LABEL;
    label->text = strdup(text);
    label->tff_font = TTF_OpenFont(font_path, font_size);
    label->sdl_color = color;
    label->sdl_surface = TTF_RenderText_Solid(label->tff_font, label->text, label->sdl_color);
    label->sdl_texture = SDL_CreateTextureFromSurface(window->sdl_renderer, label->sdl_surface);
    label->sdl_rect = (SDL_Rect) {0, 0, label->sdl_surface->w, label->sdl_surface->h};
    label->x = 0;
    label->y = 0;

    SDL_SetTextureBlendMode(label->sdl_texture, SDL_BLENDMODE_BLEND);

    return label;
}

void catpong_label_free(catpong_label_t *label) {
    free(label->text);
    TTF_CloseFont(label->tff_font);
    SDL_FreeSurface(label->sdl_surface);
    SDL_DestroyTexture(label->sdl_texture);
    free(label);
}

void catpong_label_render(catpong_label_t *label, int x, int y) {
    label->sdl_rect.x = x;
    label->sdl_rect.y = y;

    SDL_RenderCopy(label->window->sdl_renderer, label->sdl_texture, NULL, &label->sdl_rect);
}

void catpong_label_update(catpong_label_t *label) {
    catpong_label_render(label, label->x, label->y);
}

void catpong_label_set_position(catpong_label_t *label, int x, int y) {
    label->x = x;
    label->y = y;
    label->sdl_rect.x = x;
    label->sdl_rect.y = y;
}

void catpong_label_set_text(catpong_label_t *label, const char *text) {
    free(label->text);
    SDL_FreeSurface(label->sdl_surface);
    SDL_DestroyTexture(label->sdl_texture);

    label->text = strdup(text);

    label->sdl_surface = TTF_RenderText_Solid(label->tff_font, label->text, label->sdl_color);
    label->sdl_texture = SDL_CreateTextureFromSurface(label->window->sdl_renderer, label->sdl_surface);

    SDL_SetTextureBlendMode(label->sdl_texture, SDL_BLENDMODE_BLEND);
}

catpong_button_t* catpong_button_new(catpong_window_t* window, const char *font_path, int font_size, const char *text, SDL_Color color, SDL_Color background_color) {
    catpong_button_t *button = malloc(sizeof(catpong_button_t));
    button->window = window;
    button->node.type = CATPONG_NODE_TYPE_BUTTON;
    button->text = strdup(text);
    button->width = 100;
    button->height = 50;
    button->tff_font = TTF_OpenFont(font_path, font_size);
    button->sdl_color = color;
    button->sdl_background_color = background_color;
    button->sdl_surface = TTF_RenderText_Solid(button->tff_font, button->text, button->sdl_color);
    button->sdl_texture = SDL_CreateTextureFromSurface(window->sdl_renderer, button->sdl_surface);
    button->sdl_rect = (SDL_Rect) {0, 0, button->width, button->height};
    button->x = 0;
    button->y = 0;
    button->is_hovered = false;
    button->is_hovered_handled = false;
    button->is_pressed = false;
    button->is_pressed_handled = false;

    SDL_SetTextureBlendMode(button->sdl_texture, SDL_BLENDMODE_BLEND);

    return button;
}

void catpong_button_free(catpong_button_t *button) {
    free(button->text);
    TTF_CloseFont(button->tff_font);
    SDL_FreeSurface(button->sdl_surface);
    SDL_DestroyTexture(button->sdl_texture);
    free(button);
}

void catpong_button_render(catpong_button_t *button) {
    SDL_SetRenderDrawColor(button->window->sdl_renderer, button->sdl_background_color.r, button->sdl_background_color.g, button->sdl_background_color.b, button->sdl_background_color.a);
    SDL_RenderFillRect(button->window->sdl_renderer, &button->sdl_rect);

    SDL_SetTextureColorMod(button->sdl_texture, button->sdl_color.r, button->sdl_color.g, button->sdl_color.b);
    SDL_SetTextureAlphaMod(button->sdl_texture, button->sdl_color.a);

    SDL_RenderCopy(button->window->sdl_renderer, button->sdl_texture, NULL, &button->sdl_rect);
}

void catpong_button_update(catpong_button_t *button) {
    if (button->is_hovered) {
        button->sdl_color = (SDL_Color) {0, 0, 0, 255};
        button->sdl_background_color = (SDL_Color) {255, 255, 255, 255};
    } else {
        button->sdl_color = (SDL_Color) {255, 255, 255, 255};
        button->sdl_background_color = (SDL_Color) {0, 0, 0, 255};
    }
    
    catpong_button_render(button);
}

void catpong_button_set_position(catpong_button_t *button, int x, int y) {
    button->x = x;
    button->y = y;
    button->sdl_rect.x = x;
    button->sdl_rect.y = y;
}

void catpong_button_set_size(catpong_button_t *button, int width, int height) {
    button->width = width;
    button->height = height;
    button->sdl_rect.w = width;
    button->sdl_rect.h = height;
    button->sdl_surface->w = width;
    button->sdl_surface->h = height;
}

void catpong_button_update_hovered(catpong_button_t *button, int x, int y) {
    button->is_hovered = x >= button->x && x <= button->x + button->width && y >= button->y && y <= button->y + button->height;

    if (!button->is_hovered) {
        button->is_hovered_handled = false;
    }
}

void catpong_button_update_pressed(catpong_button_t *button, bool is_pressed) {
    button->is_pressed = is_pressed && button->is_hovered;

    if (!is_pressed) {
        button->is_pressed_handled = false;
    }
}

bool catpong_button_is_hovered_once(catpong_button_t *button) {
    if (button->is_hovered && !button->is_hovered_handled) {
        button->is_hovered_handled = true;
        return true;
    }

    return false;
}

bool catpong_button_is_pressed_once(catpong_button_t *button) {
    if (button->is_pressed && !button->is_pressed_handled) {
        button->is_pressed_handled = true;
        return true;
    }

    return false;
}

void catpong_button_use_mouse_state(catpong_button_t *button, catpong_input_mouse_state_t *mouse_state) {
    catpong_button_update_hovered(button, mouse_state->x, mouse_state->y);
    catpong_button_update_pressed(button, !button->mouse_state.left_button && mouse_state->left_button);

    button->mouse_state = *mouse_state;
}