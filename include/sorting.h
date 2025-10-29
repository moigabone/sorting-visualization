#ifndef SORTING_H
#define SORTING_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

void bubble_sort(int* array, int size, SDL_Renderer* renderer,TTF_Font* font); 

void selection_sort(int* array, int size, SDL_Renderer* renderer,TTF_Font* font); 

void insertion_sort(int* array, int size, SDL_Renderer* renderer,TTF_Font* font); 

#endif 