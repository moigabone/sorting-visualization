#ifndef VISUAL_H
#define VISUAL_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "main.h"
#include "stats.h"

//Initializes all SDL/TTF systems and creates the app struct.
// return a pointer to the newly created App_window, or NULL on failure.
//implemented in visual.c
App_Window* initAppVisuals(); // Renamed to be specific to 'visual'

// Cleans up all resources (memory, SDL, TTF).
//param app A pointer to the App_Window struct to be freed.
//implemented in visual.c
void cleanupAppVisuals(App_Window* app);

//implemented in visual.c
void drawArray(SDL_Renderer* renderer, int* array, int size, int highlight1, int highlight2);

//implemented in visual.c
void drawStats(SDL_Renderer* renderer, TTF_Font* font, Stats_t* stats);

// (Clears screen, draws elements, and presents):
// renderer :The SDL renderer to draw on.
// array :The array of integers to visualize.
// size :The size of the array.
//implemented in visual.c
void renderApp(App_Window* app, int highlight1, int hightlight2);

//implemented in visual.c
void drawLegend(SDL_Renderer* renderer, TTF_Font* font, int selectedAlgorithm);

#endif 