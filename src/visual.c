#include "visual.h"
#include "main.h"   // For global constants (N, WINDOW_WIDTH, etc.)
#include "utils.h"  // For createRandomArray (part of init)
#include "stats.h"
#include <stdio.h>  // For error messages
#include <stdlib.h> // For malloc/free
#include <SDL2/SDL.h>

App_Window* initAppVisuals() {
    // 1. Allocate memory for the App struct itself
    App_Window* app = (App_Window*)malloc(sizeof(App_Window));
    if (app == NULL) {
        fprintf(stderr, "Failed to allocate memory for App struct.\n");
        return NULL;
    }
    // 1) Set pointers to NULL initially for safe cleanup
    app->window = NULL;
    app->renderer = NULL;
    app->font = NULL;
    app->array = NULL;


    // 2) Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL initialization failed: %s\n", SDL_GetError());
        cleanupAppVisuals(app); // Call cleanup (handles partial init)
        return NULL;
    }

    // 3) Initialize TTF
    if (TTF_Init() == -1) {
        fprintf(stderr, "TTF initialization failed: %s\n", TTF_GetError());
        cleanupAppVisuals(app);
        return NULL;
    }

    // 4) Create Window
    app->window = SDL_CreateWindow(
        "Project C - Sorting Visualization",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN
    );
    if (app->window == NULL) {
        fprintf(stderr, "Window creation failed: %s\n", SDL_GetError());
        cleanupAppVisuals(app);
        return NULL;
    }

    // 5) Create Renderer
    app->renderer = SDL_CreateRenderer(app->window, -1, SDL_RENDERER_ACCELERATED);
    if (app->renderer == NULL) {
        fprintf(stderr, "Renderer creation failed: %s\n", SDL_GetError());
        cleanupAppVisuals(app);
        return NULL;
    }

    // 6) Load Font 
    app->font = TTF_OpenFont("font.otf", 22);
    if (app->font == NULL) {
        fprintf(stderr, "Failed to load font: %s\n", TTF_GetError());
        cleanupAppVisuals(app);
        return NULL;
    }

    // 7) Create Array (Data is initialized along with visuals here)
    app->array = createRandomArray(N, WINDOW_HEIGHT); // 50px margin
    if (app->array == NULL) {
        fprintf(stderr, "Failed to create array (malloc failed).\n");
        cleanupAppVisuals(app);
        return NULL;
    }

    // Stats
    app->stats = createStats();
    if (app->stats == NULL){
        fprintf(stderr, "Failed to create stats struct.\n");
        cleanupAppVisuals(app);
        return NULL;
    }

    // Set initial state
    app->running = 1;
    app->selectedAlgorithm = 0;

    printf("SDL & TTF initialization successful!\n");
    return app;
}

void cleanupAppVisuals(App_Window* app) {
    if (app == NULL) return; 

    freeStats(app->stats);
    // Free all resources in reverse order of creation
    // Check if pointers are not NULL before freeing/destroying
    if (app->array) free(app->array);
    if (app->font) TTF_CloseFont(app->font);
    if (app->renderer) SDL_DestroyRenderer(app->renderer);
    if (app->window) SDL_DestroyWindow(app->window);
    
    // Quit SDL subsystems only after destroying components
    TTF_Quit();
    SDL_Quit();
    
    // Free the struct itself
    free(app);
    printf("Cleanup complete.\n"); 
}

// Helper function to draw text
void drawText(SDL_Renderer* renderer, TTF_Font* font, const char* text, 
              int x, int y, SDL_Color color, int isSelected) 
{
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);
    if (surface == NULL) return; 

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == NULL) {
        SDL_FreeSurface(surface);
        return; 
    }

    SDL_Rect destRect = { x, y, surface->w, surface->h };
    SDL_RenderCopy(renderer, texture, NULL, &destRect);

    if (isSelected) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_Rect borderRect = { destRect.x - 2, destRect.y - 2, destRect.w + 4, destRect.h + 4 };
        SDL_RenderDrawRect(renderer, &borderRect);
    }

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

// Draw the right-hand menu
void drawLegend(SDL_Renderer* renderer, TTF_Font* font, int selectedAlgorithm) {
    int menuX = 610; 
    
    SDL_Rect menuBg = { menuX - 10, 0, 200, WINDOW_HEIGHT };
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255); // Dark grey
    SDL_RenderFillRect(renderer, &menuBg);

    SDL_Color white = {255, 255, 255, 255};
    SDL_Color yellow = {255, 255, 0, 255};
    SDL_Color red = {255, 0, 0, 255};
    SDL_Color green = {0, 255, 127, 255};

    drawText(renderer, font, "1: Bubble Sort", menuX, 40, white, (selectedAlgorithm == 1));
    drawText(renderer, font, "2: Selection Sort", menuX, 70, white, (selectedAlgorithm == 2));
    drawText(renderer, font, "3: Insertion Sort", menuX, 100, white, (selectedAlgorithm == 3));
    drawText(renderer, font, "4: Quick Sort", menuX, 130, white, (selectedAlgorithm == 4));

    drawText(renderer, font, "S: Start Sort", menuX, 200, green, 0);
    drawText(renderer, font, "E: Stop Sort", menuX, 230, red, 0);
    drawText(renderer, font, "R: Reset Array", menuX, 260, yellow, 0);
}

// Draw the top left performance menu
void drawStats(SDL_Renderer* renderer, TTF_Font* font, Stats_t* stats) {
    
    // Define stats box position
    SDL_Rect statsBg = { 10, 10, 250, 85 };
    
    // 2. Draw semi-transparent background
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND); // Enable transparency
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 180); // Black, 180 alpha
    SDL_RenderFillRect(renderer, &statsBg);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE); // Disable transparency
    
    // Set text color
    SDL_Color white = {255, 255, 255, 255};

    //  Buffers to hold the formatted text
    char timeText[100];
    char compText[100];
    char accessText[100];
    
    //start time accumulated
    double displayTime = stats->executionTime;
    
    if (stats->startTicks != 0) {
        // Sort is RUNNING. Calculate elapsed time on the fly.
        Uint64 now = SDL_GetPerformanceCounter();
        Uint64 frequency = SDL_GetPerformanceFrequency();
        displayTime += (double)(now - stats->startTicks) / frequency;
    } 

    // Format the strings with the values
    sprintf(timeText,   "Execution time : %.5f s", displayTime);
    sprintf(compText,   "Comparisons : %lld", stats->comparisons);
    sprintf(accessText, "Memory accesses : %lld", stats->memoryAccesses);

    // Draw the text
    drawText(renderer, font, timeText,   20, 20, white, 0);
    drawText(renderer, font, compText,   20, 45, white, 0);
    drawText(renderer, font, accessText, 20, 70, white, 0);
}

// Draw the sorting bars
void drawArray(SDL_Renderer* renderer, int* array, int size, int highlight1, int highlight2) {
    int sortZoneWidth = 600; 
    int windowHeight = WINDOW_HEIGHT; 
    float barWidth = (float)sortZoneWidth / size;

    for (int i = 0; i < size; i++) {
        int barHeight = array[i];
        SDL_Rect bar;
        bar.x = (int)(i * barWidth);
        bar.w = (int)barWidth;
        bar.y = windowHeight - barHeight; 
        bar.h = barHeight;

        if (i == highlight1) {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red
        } else if (i == highlight2) {
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Green
        } else {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White
        }
        
        SDL_RenderFillRect(renderer, &bar);
    }
}

// Render everything
void renderApp(App_Window* app, int highlight1, int highlight2) 
{    
    SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, 255);
    SDL_RenderClear(app->renderer);
    drawArray(app->renderer, app->array, N, highlight1, highlight2);
    drawLegend(app->renderer, app->font, app->selectedAlgorithm);
    drawStats(app->renderer, app->font, app->stats);
    SDL_RenderPresent(app->renderer);
}