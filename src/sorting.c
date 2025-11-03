#include "sorting.h"
#include "visual.h" // for renderApp (drawing the array)
#include "utils.h"  // for handleEvents (to prevent window freeze)
#include "stats.h"
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
void bubble_sort(App_Window* app) {
    
    int running = 1; // Local flag to check if the user closes the window
    int actionCode = 0; // Need to stop the sort

    int size= N; //from main.h
    int* tab = app->array; //access to the array from the struct 

    for (int i = 0; i < size - 1; i++) {
        int swapped = 0; // Track if any swaps happened during this pass

        for (int j = 0; j < size - 1 - i; j++) {
            // --- VISUALIZATION: Comparison ---
            actionCode = handleEvents(&running); // Keep window responsive
            if (!running) { app->running = 0; return; } //stop everything
            if (actionCode == 50) return;   // Stop sorting if user closes the window

             //Stats
            app->stats->comparisons++; 
            app->stats->memoryAccesses += 2;

            
            // Redraw the array, highlighting compared elements
            // j = red, j+1 = green
            renderApp(app, j, j + 1);
            SDL_Delay(5); // Short delay to make animation visible

            // Swap if needed ---
            if (tab[j] > tab[j + 1]) {

                //stats
                app->stats->memoryAccesses += 3;

                int tmp = tab[j];
                tab[j] = tab[j + 1];
                tab[j + 1] = tmp;
                swapped = 1;

                // --- VISUALIZATION: Swap ---
                if (!running) { app->running = 0; return; }
                if (actionCode == 50) return;
                
                // Redraw immediately to show result of the swap
                renderApp(app, j, j + 1);
                SDL_Delay(5); // Slightly longer delay on swap
            }
        }

        // Optimization: stop if array is already sorted
        if (!swapped) break;
    }

    // --- FINAL VISUALIZATION: Draw sorted array ---
    renderApp(app, -1, -1); // no highlight
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
void selection_sort(App_Window* app) {

    int running = 1; // Local flag to check if the user closes the window
    int actionCode = 0; // Need to stop the sort

    int size = N;
    int* tab = app->array;

    for (int i = 0; i < size - 1; i++) {
        int minimum = i; // Index of the smallest element in the unsorted part

        // Find smallest element in the remaining array
        for (int j = i + 1; j < size; j++) {
            //stats
            app->stats->comparisons++;
            app->stats->memoryAccesses += 2; // read tab[j] and tab[minimum]


            if (tab[j] < tab[minimum]) {
                minimum = j;    
            }
        }

        // --- VISUALIZATION: Highlight current position and minimum ---
        actionCode = handleEvents(&running);
        if (!running) { app->running = 0; return; }
        if (actionCode == 50) return;
        renderApp(app, i, minimum);
        SDL_Delay(20);

        // Swap if needed
        if (minimum != i) {
            //stats
            app->stats->memoryAccesses += 4; // 4 access : 2 read, 2 write) ---

            int tmp = tab[i];
            tab[i] = tab[minimum];
            tab[minimum] = tmp;

            // --- VISUALIZATION: After swap ---
            actionCode = handleEvents(&running);
            if (!running) { app->running = 0; return; }
            if (actionCode == 50) return;
            renderApp(app, i, minimum);
            SDL_Delay(20);
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
void insertion_sort(App_Window* app) {

    int running = 1; // Local flag to check if the user closes the window
    int actionCode = 0; // Need to stop the sort

    int size = N;
    int* tab = app->array;

    for (int i = 1; i < size; i++) {
        // Stats (1 read)
        app->stats->memoryAccesses++;
        int key = tab[i]; // Element to insert
        int j = i - 1;

        // --- VISUALIZATION: Show current key and comparison position ---
        actionCode = handleEvents(&running);
        if (!running) { app->running = 0; return; }
        if (actionCode == 50) return;
        renderApp(app, j, i);
        SDL_Delay(20);

        //Stats (1 comparison, 1 read)
        app->stats->comparisons++;
        app->stats->memoryAccesses++;
        // Shift larger elements to the right
        while (j >= 0 && tab[j] > key) {
            //Stats (1 write, 1 read)
            app->stats->memoryAccesses += 2; // tab[j] already read
            tab[j + 1] = tab[j];
            j--;

            // --- VISUALIZATION: Show shifting in progress ---
            actionCode = handleEvents(&running);
            if (!running) { app->running = 0; return; }
            if (actionCode == 50) return;
            renderApp(app, j, i);
            SDL_Delay(20);
        }

        
        //Stats (1 write) 
        app->stats->memoryAccesses++;
        // Insert the key into its correct position
        tab[j + 1] = key;
    }
}



 // This function handles moving elements and updating the display.
 // It returns the new pivot index, or -1 if the user stops the sort.
static int partition(App_Window* app, int low, int high) {
    
    int* tab = app->array; // Get the array from 'app'
    int running = 1;      // (Local flag for handleEvents)
    int actionCode = 0;   // (Local var for handleEvents)

    // Choose the last element as the pivot
    app->stats->memoryAccesses++; // 1 read (pivot)
    int pivot_value = tab[high]; 
    
    int i = (low - 1); // Index of the smaller element

    for (int j = low; j < high; j++) {
        // Handle events (for 'E' or 'X')
        actionCode = handleEvents(&running);
        if (!running) { app->running = 0; return -1; } // Stop everything
        if (actionCode == 50) return -1; // Stop the sort

        // --- Stats ---
        app->stats->comparisons++;    // 1 comparison
        app->stats->memoryAccesses++; // 1 read (tab[j])

        if (tab[j] < pivot_value) {
            i++; 
            
            // --- Stats (4 accesses: 2 reads, 2 writes) ---
            app->stats->memoryAccesses += 4;
            int temp = tab[i];
            tab[i] = tab[j];
            tab[j] = temp;
        }

        // --- Visualization ---
        // Highlight 'j' (scanning) and 'high' (the pivot)
        renderApp(app, j, high);
        SDL_Delay(5);
    }

    // Place the pivot in its final position
    // --- Stats (4 accesses: 2 reads, 2 writes) ---
    app->stats->memoryAccesses += 4;
    int temp = tab[i + 1];
    tab[i + 1] = tab[high]; // tab[high] is the pivot
    tab[high] = temp;
    
    // Handle events one last time
    actionCode = handleEvents(&running);
    if (!running) { app->running = 0; return -1; }
    if (actionCode == 50) return -1;
    
    // Show the final swap
    renderApp(app, i + 1, high);
    SDL_Delay(5);

    return (i + 1); // Return the pivot's index
}


//"Private" recursive function for Quick Sort.
// This manages the divide-and-conquer logic.
 
static void quick_sort_recursive(App_Window* app, int low, int high) {
    
    // Base case for the recursion
    // Also check if the user has quit
    if (low < high && app->running) {
        
        // 1. Find the pivot
        int pivot_index = partition(app, low, high);
        
        // If partition returned -1 (user stop), abort
        if (pivot_index == -1) return;

        // 2. Sort the left side (before the pivot)
        quick_sort_recursive(app, low, pivot_index - 1);
        
        // 3. Sort the right side (after the pivot)
        quick_sort_recursive(app, pivot_index + 1, high);
    }
}

// @brief "Public" function for Quick Sort.
// This is the one called by runMainLoop (e.g., via key '4').
void quick_sort(App_Window* app) {
    int size = N; // N is global via main.h

    // 1. Start the recursive sort on the entire array (0 to size - 1)
    quick_sort_recursive(app, 0, size - 1);

    // 2. Final render (only if the sort wasn't stopped)
    if (app->running) {
        renderApp(app, -1, -1); // No highlights
    }
}