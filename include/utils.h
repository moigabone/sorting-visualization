#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h> // We need this for malloc() and free()

//SDL Events
int handleEvents(int* running);

//random number
int* createRandomArray(int size, int maxValue);


#endif