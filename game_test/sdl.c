#include "sdl.h"
#include <SDL2/SDL.h>
#include "drawing.h"
#include "controller.h"
#include <stdio.h>

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static SDL_Texture* texture = NULL;
int running = 1;

// Create an SDL window and renderer
void sdl_init() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return;
    }

    window = SDL_CreateWindow("Game Window", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA4444, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);
    if (texture == NULL) {
        printf("SDL_CreateTexture Error: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }
}

// Stop the thread and close the window
void sdl_cleanup() {
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

// Handles key presses to control tanks (in the main thread)
void sdl_handle_input() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running  = 0;
        } 
        else if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                // Player 1 controls
                case SDLK_w: p1_up = 1; break;
                case SDLK_a: p1_left = 1; break;
                case SDLK_s: p1_down = 1; break;
                case SDLK_d: p1_right = 1; break;
                case SDLK_q: p1_trig = 1; break;

                // Player 2 controls
                case SDLK_i: p2_up = 1; break;
                case SDLK_j: p2_left = 1; break;
                case SDLK_k: p2_down = 1; break;
                case SDLK_l: p2_right = 1; break;
                case SDLK_o: p2_trig = 1; break;
            }
        } else if (event.type == SDL_KEYUP) {
            switch (event.key.keysym.sym) {
                // Player 1 controls
                case SDLK_w: p1_up = 0; break;
                case SDLK_a: p1_left = 0; break;
                case SDLK_s: p1_down = 0; break;
                case SDLK_d: p1_right = 0; break;
                case SDLK_q: p1_trig = 0; break;

                // Player 2 controls
                case SDLK_i: p2_up = 0; break;
                case SDLK_j: p2_left = 0; break;
                case SDLK_k: p2_down = 0; break;
                case SDLK_l: p2_right = 0; break;
                case SDLK_o: p2_trig = 0; break;
            }
        }
    }
}

// Render the screen (in the main thread)
void sdl_update() {
    SDL_UpdateTexture(texture, NULL, screen, SCREEN_WIDTH * sizeof(uint16_t));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}