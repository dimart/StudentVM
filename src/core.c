#include "core.h"
#include "global.h"
#include "IOSystem.h"
#include "parser.h"
#include "interpretator.h"

int runVM(VM* SVM, char* fileName)
{
    Code code;
    if (!parsing(&code, fileName))
    {
        return 0;
    }
    else if (!interpret(SVM, &code))
    {
        return 0;
    }
    if (SVM->mode) getStateOfMemory(SVM->memory);
    showStack(SVM->stack);
    terminateStack(&(SVM->stack));
    free(SVM->memory.space);
    free(SVM);
    return 1;
}
