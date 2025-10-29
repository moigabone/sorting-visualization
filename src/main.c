// src/main.c
#include "main.h"    // For defining the extern constants
#include "visual.h"  // For initAppVisuals, cleanupAppVisuals
#include "utils.h"   // For runMainLoop
#include <stdlib.h>  // For EXIT_SUCCESS/FAILURE
#include <time.h>    // For time()
#include <stdio.h>   // For srand()

// Define the global constants
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int N = 100;

int main(int argc, char *argv[])
{
    // Seed the random number generator ONCE
    srand(time(NULL));

    // 1. Initialize everything (call to visual.c)
    App_Window* app = initAppVisuals();
    if (app == NULL) {
        fprintf(stderr, "Application failed to initialize. Exiting.\n");
        return EXIT_FAILURE;
    }

    // 2. Run the application loop (call to utils.c)
    runMainLoop(app);

    // 3. Clean up all resources (call to visual.c)
    cleanupAppVisuals(app);

    return EXIT_SUCCESS;
}