#include "utils.h"  
#include <time.h>   
#include <stdlib.h> // For rand() and malloc()
#include <SDL2/SDL.h>

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
                // Touche '1'
                case SDLK_1:
                case SDLK_KP_1:
                    printf("Key 1 pressed: Requesting Bubble Sort\n");
                    return 1; // actionCode for case

                // Touche '2'
                case SDLK_2:
                case SDLK_KP_2:
                    printf("Key 2 pressed: Requesting Selection Sort\n");
                    return 2; // actionCode for case
                
                // Touche '3'
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
       
        array[i] = (rand() % maxValue) + 1;  // We add +1 to avoid 0-height bars (values from 1 to maxValue)
    }
    
    // We return the pointer to the array so main.c can use it.
    return array;
}



