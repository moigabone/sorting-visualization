#include "sorting.h"
#include "visual.h" // for renderApp (drawing the array)
#include "utils.h"  // for handleEvents (to prevent window freeze)
#include "stats.h"
#include <SDL2/SDL_ttf.h>

/*
----------------------------------------------------
 BUBBLE SORT
----------------------------------------------------
 Repeatedly compares adjacent elements and swaps them if they’re in the wrong order.
 After each pass, the largest element “bubbles up” to the end of the array.
----------------------------------------------------
*/
void bubble_sort(App_Window* app) {
    int running = 1; // local flag to check if the user closes the window
    int actionCode = 0; // need to stop the sort
    int size= N; // array size from main.h
    int* tab = app->array;

    for (int i = 0; i < size - 1; i++) {
        int swapped = 0; // early-exit optimization

        for (int j = 0; j < size - 1 - i; j++) {
            // keep UI responsive; allow early stop
            actionCode = handleEvents(&running); // keep window responsive
            if (!running) { app->running = 0; return; } //stop everything
            if (actionCode == 50) { 
                Uint64 now = SDL_GetPerformanceCounter();
                Uint64 frequency = SDL_GetPerformanceFrequency();

                app->stats->executionTime += (double)(now - app->stats->startTicks) / frequency;
                app->stats->startTicks = 0; 
                return;
            }

            // stats: compare tab[j] vs tab[j+1]
            app->stats->comparisons++; 
            app->stats->memoryAccesses += 2;

            // visual: highlight j (red) and j+1 (green)
            renderApp(app, j, j + 1);
            SDL_Delay(1); // short delay to make animation visible

            // swap if out of order
            if (tab[j] > tab[j + 1]) {
                app->stats->memoryAccesses += 3; // tmp read+write + one extra read/write pair approximated

                int tmp = tab[j];
                tab[j] = tab[j + 1];
                tab[j + 1] = tmp;
                swapped = 1;

                // visual : show swap result
                if (!running) { app->running = 0; return; }
                if (actionCode == 50) { 
                    Uint64 now = SDL_GetPerformanceCounter();
                    Uint64 frequency = SDL_GetPerformanceFrequency();

                    app->stats->executionTime += (double)(now - app->stats->startTicks) / frequency;
                    app->stats->startTicks = 0; 
                    return;
                }
                
                renderApp(app, j, j + 1);
                SDL_Delay(1);
            }
        }

        if (!swapped) break; // already sorted
    }

    // Final clean frame (no highlights)
    renderApp(app, -1, -1);
}

/*
----------------------------------------------------
 SELECTION SORT VISUALIZATION
----------------------------------------------------
Scans the array to find the smallest element and places it at the beginning.
Repeats the process for the remaining unsorted part.
----------------------------------------------------
*/
void selection_sort(App_Window* app) {
    int running = 1;
    int actionCode = 0;
    int size = N;
    int* tab = app->array;

    for (int i = 0; i < size - 1; i++) {
        int minimum = i; // Index of the smallest element in the unsorted part

        // Find smallest element in the remaining array
        for (int j = i + 1; j < size; j++) {
            app->stats->comparisons++;
            app->stats->memoryAccesses += 2; // read tab[j] and tab[minimum]

            if (tab[j] < tab[minimum]) {
                minimum = j;    
            }
        }

        // visual: show current i and candidate minimum
        actionCode = handleEvents(&running);
        if (!running) { app->running = 0; return; }
        if (actionCode == 50) { 
            Uint64 now = SDL_GetPerformanceCounter();
            Uint64 frequency = SDL_GetPerformanceFrequency();

            app->stats->executionTime += (double)(now - app->stats->startTicks) / frequency;
            app->stats->startTicks = 0; 
            return;
            }
        renderApp(app, i, minimum);
        SDL_Delay(20);

        // place the minimum at position i
        if (minimum != i) {
            app->stats->memoryAccesses += 4; // 2 reads + 2 writes for swap

            int tmp = tab[i];
            tab[i] = tab[minimum];
            tab[minimum] = tmp;

            // visual: show after-swap state
            actionCode = handleEvents(&running);
            if (!running) { app->running = 0; return; }
            if (actionCode == 50) { 
                Uint64 now = SDL_GetPerformanceCounter();
                Uint64 frequency = SDL_GetPerformanceFrequency();

                app->stats->executionTime += (double)(now - app->stats->startTicks) / frequency;
                app->stats->startTicks = 0; 
                return;
            }
            renderApp(app, i, minimum);
            SDL_Delay(20);
        }   
    }
}

/*
----------------------------------------------------
 INSERTION SORT VISUALIZATION
----------------------------------------------------
Builds the sorted part of the array one element at a time.
Each new element is inserted into its correct position within the sorted portion.
----------------------------------------------------
*/
void insertion_sort(App_Window* app) {

    int running = 1;
    int actionCode = 0;
    int size = N;
    int* tab = app->array;

    for (int i = 1; i < size; i++) {
        app->stats->memoryAccesses++; // read key
        int key = tab[i];
        int j = i - 1;

        // visual: show (j, i) before shifting
        actionCode = handleEvents(&running);
        if (!running) { app->running = 0; return; }
        if (actionCode == 50) { 
            Uint64 now = SDL_GetPerformanceCounter();
            Uint64 frequency = SDL_GetPerformanceFrequency();

            app->stats->executionTime += (double)(now - app->stats->startTicks) / frequency;
            app->stats->startTicks = 0; 
            return;
        }
        renderApp(app, j, i);
        SDL_Delay(5);

        // first comparison counted here:
        app->stats->comparisons++;
        app->stats->memoryAccesses++;

        // shift larger elements to the right
        while (j >= 0 && tab[j] > key) {
            app->stats->memoryAccesses += 2; // write tab[j+1], read tab[j] already accounted loosely
            tab[j + 1] = tab[j];
            j--;

            // visual: show shifting progress (j moves left)
            actionCode = handleEvents(&running);
            if (!running) { app->running = 0; return; }
            if (actionCode == 50) { 
                Uint64 now = SDL_GetPerformanceCounter();
                Uint64 frequency = SDL_GetPerformanceFrequency();

                app->stats->executionTime += (double)(now - app->stats->startTicks) / frequency;
                app->stats->startTicks = 0; 
                return;
            }
            renderApp(app, j, i);
            SDL_Delay(5);

            // (optional accuracy) count next comparison for next loop test:
            // app->stats->comparisons += 1; app->stats->memoryAccesses += 1;
        }

        // insert the key at its position
        app->stats->memoryAccesses++; // write key
        tab[j + 1] = key;
    }
}

/*
----------------------------------------------------
 INSERTION SORT VISUALIZATION
----------------------------------------------------
Chooses a pivot, then partitions the array into two parts:
    -Elements smaller than the pivot
    -Elements greater than the pivot
Recursively sorts both sides.
----------------------------------------------------
*/

// Partition around pivot = tab[high]. Returns pivot's final index.
// Returns -1 if the user stops/quit to abort the sort gracefully.
static int partition(App_Window* app, int low, int high) {
    int* tab = app->array;
    int running = 1;
    int actionCode = 0;

    app->stats->memoryAccesses++; // read pivot
    int pivot_value = tab[high]; 
    
    int i = (low - 1); // boundary of "elements < pivot"

    for (int j = low; j < high; j++) {
        actionCode = handleEvents(&running);
        if (!running) { app->running = 0; return -1; }
        if (actionCode == 50) { 
            Uint64 now = SDL_GetPerformanceCounter();
            Uint64 frequency = SDL_GetPerformanceFrequency();

            app->stats->executionTime += (double)(now - app->stats->startTicks) / frequency;
            app->stats->startTicks = 0; 
            return -1;
        }

        app->stats->comparisons++;
        app->stats->memoryAccesses++; // read tab[j]

        if (tab[j] < pivot_value) {
            i++; 
            // swap tab[i] and tab[j]
            app->stats->memoryAccesses += 4; // 2 reads + 2 writes
            int temp = tab[i];
            tab[i] = tab[j];
            tab[j] = temp;
        }

        renderApp(app, j, high);
        SDL_Delay(5);
    }

    // place pivot at i+1 (its final position)
    app->stats->memoryAccesses += 4; // swap pivot into place
    int temp = tab[i + 1];
    tab[i + 1] = tab[high];
    tab[high] = temp;
    
    // One last visual update on the final swap
    actionCode = handleEvents(&running);
    if (!running) { app->running = 0; return -1; }
    if (actionCode == 50) { 
        Uint64 now = SDL_GetPerformanceCounter();
        Uint64 frequency = SDL_GetPerformanceFrequency();

        app->stats->executionTime += (double)(now - app->stats->startTicks) / frequency;
        app->stats->startTicks = 0; 
        return -1;
    }
    
    renderApp(app, i + 1, high);
    SDL_Delay(5);

    return (i + 1);
}

// Recursive helper: sorts range [low..high] if app->running is true.
static void quick_sort_recursive(App_Window* app, int low, int high) {
    if (low < high && app->running) {   
        // 1) find the pivot
        int pivot_index = partition(app, low, high);
        if (pivot_index == -1) return;

        // 2) sort the left side (before the pivot)
        quick_sort_recursive(app, low, pivot_index - 1);
        
        // 3) sort the right side (after the pivot)
        quick_sort_recursive(app, pivot_index + 1, high);
    }
}

// public entry point for Quick Sort (called from main loop)
void quick_sort(App_Window* app) {
    int size = N; // global length from main.h

    // 1) start the recursive sort on the entire array (0 to size - 1)
    quick_sort_recursive(app, 0, size - 1);

    // 2) final render (only if the sort wasn't stopped)
    if (app->running) {
        renderApp(app, -1, -1);
    }
}