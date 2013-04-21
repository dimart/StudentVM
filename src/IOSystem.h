#ifndef IOSYSTEM_H
#define IOSYSTEM_H
#include "memory.h"
//#include "global.h"

int readString(FILE **input, char *string, int *error);
int openFile(FILE **input, const char *fileName);
int findSubstr(char s[], char p[]);
void checkConfig(Config* c, int* errors);
int checkArgs(int argc, char *argv[], char *mode);
void showStack(Stack *s);

#endif
