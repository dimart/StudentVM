#ifndef STRUCTURES_H
#define STRUCTURES_H

#include "global.h"

typedef
struct {
    FILE* file;
    unsigned long int memorySize;
} Config;

typedef
struct {
    long int *space;
    unsigned long int currentAddress;
} Memory;

typedef
struct Stack {
    long int data;
    struct Stack *next;
} Stack;

typedef
struct {
    Memory memory;
    Stack *stack;
    Config config;
    int errorCode;
    char mode;
} VM;

typedef
enum {
    LDC = 1,
    LDI,
    LD,
    ST,
    STI,
    ADD,
    SUB,
    MUL,
    DIV,
    MOD,
    CMP,
    JMP,
    BR,
    LBL,
    HLT,
    NUM,
    ERR
} Opcode;

typedef
struct {
    Opcode opcode;
    union {
        long int number;
        unsigned long int address;
        char* label;
    } args;
    char* labeldef;
    int labelLine;
} Command;

typedef
struct {
    FILE* file;
    unsigned int currentCountOfLines;
    unsigned int countOfLabels;
    Command* commands;
} Code;

typedef
struct {
    char* labdef;
    int lineNumber;
} Label;

typedef
struct {
    Label *label;
} TableOfLabels;

typedef
struct {
    char* line;
    int current;
    enum {
        twopartAdr = 17,
        twopartNum,
        twopartLab,
        onepart,
        comment,
        NUL
    } type;
} Expression;

#endif // STRUCTURES_H
