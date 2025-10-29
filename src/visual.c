// src/visual.c
#include "visual.h"
#include "main.h"
#include <SDL2/SDL_ttf.h>

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

    // If selected, draw a white border
    if (isSelected) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        // Add a small padding to the border
        SDL_Rect borderRect = { destRect.x - 2, destRect.y - 2, destRect.w + 4, destRect.h + 4 };
        SDL_RenderDrawRect(renderer, &borderRect);
    }

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

// Draw the right-hand menu
void drawLegend(SDL_Renderer* renderer, TTF_Font* font, int selectedAlgorithm) {
    int menuX = 610; 
    
    // 1. Draw menu background
    SDL_Rect menuBg = { menuX - 10, 0, 200, WINDOW_HEIGHT };
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255); // Dark grey
    SDL_RenderFillRect(renderer, &menuBg);

    // 2. Define colors
    SDL_Color white = {255, 255, 255, 255};
    SDL_Color yellow = {255, 255, 0, 255};

    // 3. Draw sorting options
    drawText(renderer, font, "1: Bubble Sort", menuX, 40, white, (selectedAlgorithm == 1));
    drawText(renderer, font, "2: Selection Sort", menuX, 70, white, (selectedAlgorithm == 2));
    drawText(renderer, font, "3: Insertion Sort", menuX, 100, white, (selectedAlgorithm == 3));
    // ... (add 4 and 5 here) ...

    // 4. Draw instructions
    drawText(renderer, font, "S: Start Sort", menuX, 200, yellow, 0);
    drawText(renderer, font, "R: Reset Array", menuX, 230, yellow, 0);
}

// --- THIS IS THE CORRECTED FUNCTION ---
void drawArray(SDL_Renderer* renderer, int* array, int size, int highlight1, int highlight2) {
    
    int sortZoneWidth = 600; // Bars stop at 600px
    // Get height from the global constant
    int windowHeight = WINDOW_HEIGHT; 
    
    float barWidth = (float)sortZoneWidth / size;

    // Loop through each element
    for (int i = 0; i < size; i++) {
        
        // --- THIS CODE WAS MISSING ---
        int barHeight = array[i];
        
        SDL_Rect bar;
        bar.x = (int)(i * barWidth);
        bar.w = (int)barWidth;
        bar.y = windowHeight - barHeight; // Y-axis is 0 at the top
        bar.h = barHeight;

        // Set color based on highlight status
        if (i == highlight1) {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red
        } else if (i == highlight2) {
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Green
        } else {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White
        }
        
        SDL_RenderFillRect(renderer, &bar);
        // --- END OF MISSING CODE ---
    }
}

// This function renders all components
void renderApp(SDL_Renderer* renderer, TTF_Font* font, int* array, int size, 
               int highlight1, int highlight2, int selectedAlgorithm) 
{    
    // Clear the whole screen to black
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // 2. Draw the bars
    drawArray(renderer, array, size, highlight1, highlight2);

    // 3. Draw the legend on top
    drawLegend(renderer, font, selectedAlgorithm);

    // 4. Present to the window
    SDL_RenderPresent(renderer);
}