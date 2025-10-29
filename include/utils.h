#ifndef UTILS_H
#define UTILS_H

#include "main.h"

//Runs the main application loop.
//This function only returns when the user quits.
//param app A pointer to the initialized App_window struct.
//implemented in utils.c
void runMainLoop(App_Window* app);


//SDL Events
//implemented in utils.c
int handleEvents(int* running);

//random number
//implemented in utils.c   
int* createRandomArray(int size, int maxValue);


#endif