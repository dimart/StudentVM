#ifndef INTERPRETATOR_H
#define INTERPRETATOR_H

#include "structures.h"
#include "stack.h"
#include "memory.h"

int interpret(VM* SVM, const Code* code);
int jmp(const Code *code, const TableOfLabels *table, unsigned int *i);
#endif // INTERPRETATOR_H
