
#include "sorting.h" 
#include "stats.h"   
#include "visual.h" 
#include "utils.h"   
#include "main.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

// Window dimensions 
const int WINDOW_WIDTH= 800;
const int WINDOW_HEIGHT= 600;

// Array size
const int N=100;

int main(int argc, char *argv[])
{

    // Seed the random number generator ONCE at the start of main
    srand(time(NULL));

    // Initialization
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        // If error, print it
        fprintf(stderr, "SDL initialization failed: %s\n", SDL_GetError());
        return 1; // Return an error code
    }

    //TTF Init
    if (TTF_Init() == -1) {
        fprintf(stderr, "TTF initialization failed: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    // Window creation
    SDL_Window* window = SDL_CreateWindow(
        "Project C - Sorting Visualization", // Title of the Window
        SDL_WINDOWPOS_CENTERED,              // Position X (middle)
        SDL_WINDOWPOS_CENTERED,              // Position Y (middle)
        WINDOW_WIDTH,                        // Width (800)
        WINDOW_HEIGHT,                       // Height (600)
        SDL_WINDOW_SHOWN                     // visible window
    );

    // Check if the window creation failed
    if (window == NULL) {
        fprintf(stderr, "Window creation failed: %s\n", SDL_GetError());
        SDL_Quit(); // Clean up SDL and exit
        return 1;
    }

    // Create a renderer
    // allow to draw (bars, colors, etc.) in the window.
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Check if the renderer creation failed (This check was missing)
    if (renderer == NULL) {
        fprintf(stderr, "Renderer creation failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(window); // Clean up the window
        SDL_Quit();                // Clean up SDL
        return 1;
    }

    // Font Loading
    TTF_Font* font = TTF_OpenFont("font.otf", 18); // load 'font.otf, 18 is the height
    if (font == NULL) {
        fprintf(stderr, "Failed to load font: %s\n", TTF_GetError());
        // ... (nettoyage SDL, renderer, window) ...
        TTF_Quit();
        return 1;
    }

    // Create the array (using utils.c)
    // We use (HEIGHT - 50) to leave some margin at the top
    int* array = createRandomArray(N, WINDOW_HEIGHT - 1);

    // Check if the array creation failed
    if (array == NULL) {
        // Malloc failure doesn't set an SDL error, so we use a custom message
        fprintf(stderr, "Failed to create array (malloc failed).\n");
        SDL_DestroyRenderer(renderer); // Clean up the renderer
        SDL_DestroyWindow(window);     // Clean up the window
        SDL_Quit();                    // Clean up SDL
        return 1;
    }

    printf("SDL & TTF initialization successful! 800x600 window created.\n");
    printf("Press 1 for Bubble Sort, 2 for Selection Sort, 3 for Insersion. Then 'S' to start. R to Reset.\n");

    // selection_sort(array, N, renderer);

    // --- MAIN LOOP ---
    int running = 1;   // flag to control the loop. 1 = running
    int actionCode=0;
    int selectedAlgorithm=0;

    while (running) {
        
        //  EVENT HANDLING (call function from utils.c)
        actionCode=handleEvents(&running);
        
        if (actionCode > 0 && actionCode < 10) {
            selectedAlgorithm = actionCode;
        }
        else if (actionCode == 99) { // 'R' = Reset
            free(array);
            array = createRandomArray(N, WINDOW_HEIGHT - 50);
            selectedAlgorithm = 0;
        }
        else if (actionCode == 100) { // 'S' = Start
            switch (selectedAlgorithm) {
                case 1: // Bubble Sort
                bubble_sort(array, N, renderer, font); // start the bubble visualisation 
                break;
            case 2: // Selection Sort
                 selection_sort(array, N, renderer, font); // start the selection visualisation 
                break;
            case 3: // Insertion Sort
                insertion_sort(array, N, renderer, font); // (Quand vous l'aurez écrit)
                break;
            
            case 99: // Reset array
                // Clear the old array
                free(array);
                // New random array
                array = createRandomArray(N, WINDOW_HEIGHT - 1);
                if (array == NULL) {
                    fprintf(stderr, "Failed to reset array.\n");
                    running = 0; // Leave
                }
                break;
            }

        }
        // DRAWING (call function from visual.c)
        renderApp(renderer,font,array, N, -1, -1,selectedAlgorithm);

        SDL_Delay(30);  
    }
    // --- END OF MAIN LOOP ---


    // Clean up and exit
    free(array);                   // Free the array memory
    TTF_CloseFont(font);           // Free the font
    SDL_DestroyRenderer(renderer); // Destroy the renderer before leaving
    SDL_DestroyWindow(window);     // Destroy the window
    SDL_Quit();                    // Quit SDL subsystems

    return 0; // Success
}