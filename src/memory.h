#ifndef MEMORY_H
#define MEMORY_H

#include "structures.h"

long int *createMemory(unsigned long int s);
int loadDataToMemory(Memory* memory, long int data, unsigned long int address);
long int getDataFromMemory(Memory m, unsigned long int a);
void getStateOfMemory(Memory m);

#endif
