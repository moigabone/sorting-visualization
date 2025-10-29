#include "sorting.h"
#include "visual.h" // for renderApp (drawing the array)
#include "utils.h"  // for handleEvents (to prevent window freeze)
#include <SDL2/SDL_ttf.h>

/*
----------------------------------------------------
 BUBBLE SORT VISUALIZATION
----------------------------------------------------
 Step-by-step Bubble Sort implementation.
 After each comparison or swap:
  - The screen is updated (bars redrawn)
  - Compared elements are highlighted
  - Small delay added for smooth animation
----------------------------------------------------
*/
void bubble_sort(int *tab, int size, SDL_Renderer* renderer, TTF_Font* font) {
    
    int running = 1; // Local flag to check if the user closes the window

    for (int i = 0; i < size - 1; i++) {
        int swapped = 0; // Track if any swaps happened during this pass

        for (int j = 0; j < size - 1 - i; j++) {

            // --- VISUALIZATION: Comparison ---
            handleEvents(&running); // Keep window responsive
            if (!running) return;   // Stop sorting if user closes the window
            
            // Redraw the array, highlighting compared elements
            // j = red, j+1 = green
            renderApp(renderer, font, tab, size, j, j + 1, 1);
            SDL_Delay(5); // Short delay to make animation visible

            // --- LOGIC: Swap if needed ---
            if (tab[j] > tab[j + 1]) {
                int tmp = tab[j];
                tab[j] = tab[j + 1];
                tab[j + 1] = tmp;
                swapped = 1;

                // --- VISUALIZATION: Swap ---
                handleEvents(&running);
                if (!running) return;
                
                // Redraw immediately to show result of the swap
                renderApp(renderer, font, tab, size, j, j + 1, 1);
                SDL_Delay(10); // Slightly longer delay on swap
            }
        }

        // Optimization: stop if array is already sorted
        if (!swapped) break;
    }

    // --- FINAL VISUALIZATION: Draw sorted array ---
    renderApp(renderer, font, tab, size, -1, -1, 1); // no highlight
}


/*
----------------------------------------------------
 SELECTION SORT VISUALIZATION
----------------------------------------------------
 For each position i:
  - Find the smallest element in the rest of the array
  - Swap it with element at position i
 Visual updates happen after comparisons and swaps.
----------------------------------------------------
*/
void selection_sort(int *tab, int size, SDL_Renderer* renderer, TTF_Font* font) {

    int running = 1; // Local flag to check if the user closes the window

    for (int i = 0; i < size - 1; i++) {
        int minimum = i; // Index of the smallest element in the unsorted part

        // Find smallest element in the remaining array
        for (int j = i + 1; j < size; j++) {
            if (tab[j] < tab[minimum]) {
                minimum = j;
            }
        }

        // --- VISUALIZATION: Highlight current position and minimum ---
        handleEvents(&running);
        if (!running) return;
        renderApp(renderer, font, tab, size, i, minimum, 1);
        SDL_Delay(150);

        // Swap if needed
        if (minimum != i) {
            int tmp = tab[i];
            tab[i] = tab[minimum];
            tab[minimum] = tmp;

            // --- VISUALIZATION: After swap ---
            handleEvents(&running);
            if (!running) return;
            renderApp(renderer, font, tab, size, i, minimum, 1);
            SDL_Delay(150);
        }   
    }
}


/*
----------------------------------------------------
 INSERTION SORT VISUALIZATION
----------------------------------------------------
 Builds a sorted part of the array from left to right:
  - Takes the next element (key)
  - Moves larger elements one step to the right
  - Inserts key into its correct position
 Visual updates show the shifting process in real time.
----------------------------------------------------
*/
void insertion_sort(int *tab, int size, SDL_Renderer* renderer, TTF_Font* font) {

    int running = 1; // Local flag to check if the user closes the window

    for (int i = 1; i < size; i++) {
        int key = tab[i]; // Element to insert
        int j = i - 1;

        // --- VISUALIZATION: Show current key and comparison position ---
        handleEvents(&running);
        if (!running) return;
        renderApp(renderer, font, tab, size, j, i, 1);
        SDL_Delay(150);

        // Shift larger elements to the right
        while (j >= 0 && tab[j] > key) {
            tab[j + 1] = tab[j];
            j--;

            // --- VISUALIZATION: Show shifting in progress ---
            handleEvents(&running);
            if (!running) return;
            renderApp(renderer, font, tab, size, j, i, 1);
            SDL_Delay(150);
        }

        // Insert the key into its correct position
        tab[j + 1] = key;
    }
}
