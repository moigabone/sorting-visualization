#ifndef SORTING_H
#define SORTING_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

//implemented in sorting.c
void bubble_sort(int* array, int size, SDL_Renderer* renderer,TTF_Font* font); 

//implemented in sorting.c
void selection_sort(int* array, int size, SDL_Renderer* renderer,TTF_Font* font); 

//implemented in sorting.c
void insertion_sort(int* array, int size, SDL_Renderer* renderer,TTF_Font* font);

//implemented in sorting.c
void quick_sort(int* array, int size, SDL_Renderer* renderer,TTF_Font* font);

#endif 