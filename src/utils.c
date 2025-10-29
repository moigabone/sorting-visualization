#include "utils.h"
#include "main.h"
#include "visual.h"
#include "sorting.h"
#include "stats.h"
#include <time.h>   
#include <stdlib.h> // For rand() and malloc()
#include <SDL2/SDL.h>

void runMainLoop(App_Window* app) {
    int actionCode = 0;

    printf("Press 1, 2, or 3. Then 'S' to Start. 'R' to Reset. 'E' to Stop. '\n");
    
    while (app->running) {
        
        // 1. EVENT HANDLING
        actionCode = handleEvents(&app->running);
        
        // 2. LOGIC
        if (actionCode > 0 && actionCode < 10) {
            app->selectedAlgorithm = actionCode;
        }
        else if (actionCode == 99) { // 'R' = Reset
            free(app->array);
            app->array = createRandomArray(N, WINDOW_HEIGHT);
            app->selectedAlgorithm = 0;
            if (app->array == NULL) {
                fprintf(stderr, "Failed to reset array.\n");
                app->running = 0; // Exit on error
            resetStats(app->stats);
            }
        }

        else if (actionCode == 100) { // 'S' = Start
            // Reset stats and start timer
            resetStats(app->stats); 
            Uint64 startTicks = SDL_GetPerformanceCounter();
            
            switch (app->selectedAlgorithm) {
                case 1:
                    bubble_sort(app); 
                    break;
                case 2:
                    selection_sort(app); 
                    break;
                case 3:
                    insertion_sort(app);
                    break;
                default:
                    printf("No algorithm selected!\n");
                    break;
            }
            //The the timer and save the time
            Uint64 endTicks = SDL_GetPerformanceCounter();
            Uint64 frequency = SDL_GetPerformanceFrequency();
            
            // save the time into the stats struct
            app->stats->executionTime = (double)(endTicks - startTicks) / frequency;
        }

        // 3. DRAWING
        renderApp(app, -1, -1);

        SDL_Delay(16);
    }
}

int handleEvents(int* running) {
    SDL_Event event;
    
    // Check for pending events
    while (SDL_PollEvent(&event)) {
        
        // If the user clicks the 'X' button
        if (event.type == SDL_QUIT) {
            *running = 0;
            running=0;  // no action
        }
        
       // Action with keyboard
        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                // case '1'
                case SDLK_1:
                case SDLK_KP_1:
                    printf("Key 1 pressed: Requesting Bubble Sort\n");
                    return 1; // actionCode for case

                // case '2'
                case SDLK_2:
                case SDLK_KP_2:
                    printf("Key 2 pressed: Requesting Selection Sort\n");
                    return 2; // actionCode for case
                
                // case '3'
                case SDLK_3:
                case SDLK_KP_3:
                    printf("Key 3 pressed: Requesting Insertion Sort\n");
                    return 3; // actionCode for case
                
                // ... SDLK_4  SDLK_5 QuickSort MergeSort) ...

                // 'r' to unsort the window
                case SDLK_r:
                    printf("Key R pressed: Requesting array reset\n");
                    return 99; // actionCode for case

                case SDLK_s:
                    printf("Key S pressed: Starting Sort\n");
                    return 100; // actionCode for case

                case SDLK_e:
                    printf("Key E pressed: Stopping Sort\n");
                    return 50;
            }
        }   
    }
    return 0; // no action
}


int* createRandomArray(int size, int maxValue) {
    
    int* array = (int*)malloc(size * sizeof(int));

    // Always check if memory allocation was successful
    if (array == NULL) {
        return NULL; // Return NULL (error) if malloc failed
    }

    //loop to fill the array
    for (int i = 0; i < size; i++) {
       
        //new 'random', lead to have a perfect line after the sort and not an irregular line
        array[i] = (int)(((double)i / (size - 1)) * (maxValue - 1)) + 1;
        //array[i] = (rand() % maxValue) + 1;  // We add +1 to avoid 0-height bars (values from 1 to maxValue) // OLD RANDOM
    }
    // Shuffle
    // Start from last and shuffle
    for (int i = size - 1; i > 0; i--) {
        int j = rand() % (i + 1); 
        // switch array[i] with index j
        int temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
    // We return the pointer to the array so main.c can use it.
    return array;
}



