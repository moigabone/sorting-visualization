#include "stats.h"
#include <stdlib.h> // For malloc and free
#include <string.h> // For memset (to zero-out memory)

// Allocates memory for Stats_t structure and initializes it.
Stats_t* createStats() {
    // Allocate memory on the heap for the structure
    Stats_t* stats = (Stats_t*)malloc(sizeof(Stats_t));
    if (stats == NULL) {
        // Return NULL if malloc failed
        return NULL; 
    }
    
    // Initialize all counters to zero
    resetStats(stats);
    
    // Return the pointer to the new struct
    return stats;
}

// Resets all counters within the Stats_t structure to zero.
void resetStats(Stats_t* stats) {
    if (stats == NULL) {
        // Safety check, do nothing if the pointer is invalid
        return;
    }
    
    // Use memset to quickly set all bytes in the struct to 0
    // This efficiently zeroes out executionTime, comparisons, and memoryAccesses
    memset(stats, 0, sizeof(Stats_t));
}

// Frees the memory allocated for the Stats_t structure.
// A pointer to the Stats_t struct to be freed.
void freeStats(Stats_t* stats) {
    // free() can safely handle a NULL pointer,
    // but we can also check just in case.
    if (stats != NULL) {
        free(stats);
    }
}