#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "memory.h"
//#include "IOSystem.h"
//#include "structures.h"
//#include "stack.h"

/* -- DEFINE BLOCK -- */
#define DEF_MEMORY_SIZE 0x1FFFFF // 2^21 - 1 = 2097151
#define DEF_COUNT_OF_CODLINE 128
#define MAX_MEMORY_SIZE 0xFFFFFFFF // 2^32 - 1 = 4294967295
#define NAME_OF_CONFIG "StudentVM.ini"
#define SUPPLIER 16
#define MAX_LENGTH_OF_LABEL 128
#define MAX_COUNT_OF_LABELS 64

//ERRORS
#define MEMORY_ALLOCATION_ERROR 1
#define NOT_ENOUGH_ELEMENTS 2
#define SYNTAX_ERROR 3
#define CANNOT_OPEN_FILE 4
#define CANNOT_OPEN_CODE_FILE 5
#define NONEXISTENT_LABEL 6
#define NO_HLT 7
#define OUT_OF_BOUNDS_ADDRESS 8
#define DEVISION_BY_ZERO 9
