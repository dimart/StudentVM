#ifndef STACK_H
#define STACK_H

#include "structures.h"

Stack *initStack(void);
void push(Stack **p, long int x);
long int pop(Stack **p);
int isEmpty(Stack *s);
int stackSize(Stack *s);
void swap(Stack **p);
void terminateStack(Stack **s);

#endif
