#include "exceptionHandler.h"
#include "global.h"

void error(int errorCode)
{
    switch(errorCode)
    {
    case MEMORY_ALLOCATION_ERROR:
        fprintf(stderr, "Error: Cannot allocate memory.\n");
        break;
    case NOT_ENOUGH_ELEMENTS:
        fprintf(stderr, "Not enough elements in the stack for this operation. \n");
        break;
    case SYNTAX_ERROR:
        fprintf(stderr, "Check your code. \n");
        break;
    case CANNOT_OPEN_FILE:
        fprintf(stderr, "Error: Cannot open configuration file. \n");
        break;
    case CANNOT_OPEN_CODE_FILE:
        fprintf(stderr, "Error: File does not exist. \n");
        break;
    case NONEXISTENT_LABEL:
        fprintf(stderr, "Check labels names. \n");
        break;
    case DEVISION_BY_ZERO:
        fprintf(stderr, "Error: Division by zero. \n");
    case OUT_OF_BOUNDS_ADDRESS:
        fprintf(stderr, "Error: refer to the memory with a negative address or out of bounds of available memory.\n");
        break;
    case NO_HLT:
        fprintf(stderr, "Warning: 'HLT' is not found. \n");
        break;
    default:
        fprintf(stderr, "Unknown error. \n");
        break;
    }
}
