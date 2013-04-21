#ifndef PARSER_H
#define PARSER_H

#include "structures.h"

int parsing(Code* code, const char *fileName);
int parseLine(Code *code, Expression *expr, int *num);
Command expression(Expression *expr);
Command next(Expression *expr);
Command getNumber(Expression *expr);
Command getCommand(Expression *expr);
Command getLabel(Expression *expr);
char* concat(char *string, char ch);

#endif // PARSER_H
