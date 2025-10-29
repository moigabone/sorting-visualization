#ifndef MAIN_H
#define MAIN_H

// include these here so the App_Window struct understands the types
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "stats.h" 

typedef struct {
    // SDL components
    SDL_Window* window;
    SDL_Renderer* renderer;
    TTF_Font* font;

    // Application data
    int* array;
    
    // Application state
    int running;
    int selectedAlgorithm;

    Stats_t* stats;

} App_Window;

// Global constants (define in main.c)
extern const int WINDOW_WIDTH;
extern const int WINDOW_HEIGHT;
extern const int N; // array size

#endif