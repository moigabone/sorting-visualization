#ifndef STATS_H
#define STATS_H

//struct for the timer
typedef struct {
    double executionTime;
    long long comparisons;
    long long memoryAccesses;
} Stats_t;

// --- Fonctions (Implémentées dans stats.c) ---

// Implemented in stats.c)
//return a pointer to the struct Stats_t.
Stats_t* createStats();

// reset counter to 0
// Implemented in stats.c
// pointer to the structure to reset.
void resetStats(Stats_t* stats);

// free the memory of the Stats struct
// Implemented in stats.c)
// pointer to the structure to free
void freeStats(Stats_t* stats);

#endif // STATS_H