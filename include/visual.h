#ifndef VISUAL_H
#define VISUAL_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

void drawArray(SDL_Renderer* renderer, int* array, int size, int highlight1, int highlight2);


 // (Clears screen, draws elements, and presents):
 // renderer :The SDL renderer to draw on.
 // array :The array of integers to visualize.
 // size :The size of the array.

void renderApp(SDL_Renderer* renderer, TTF_Font* font, int* array, int size, int highlight1, int hightlight2, int selectedAlgorithm);

void drawLegend(SDL_Renderer* renderer, TTF_Font* font, int selectedAlgorithm);

#endif 