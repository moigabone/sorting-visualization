#ifndef STATS_H
#define STATS_H

#include <SDL2/SDL_stdinc.h> // for Uint64

//Structure to hold all performance stats
typedef struct {
    double executionTime;
    long long comparisons;
    long long memoryAccesses;
    Uint64 startTicks;
} Stats_t;


// Implemented in stats.c)
//return a pointer to the struct Stats_t.
Stats_t* createStats();

// reset counter to 0
// Implemented in stats.c
// pointer to the structure to reset.
void resetStats(Stats_t* stats);

// frees the memory of the Stats struct
// Implemented in stats.c)
void freeStats(Stats_t* stats);

#endif // STATS_H