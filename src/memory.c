#include "memory.h"
#include "exceptionHandler.h"
#include "global.h"

long int *createMemory(unsigned long int size)
{
    long int *mem;
    mem = calloc(size, sizeof(long int));
    if (mem == NULL) error(MEMORY_ALLOCATION_ERROR);
    return mem;
}

long int getDataFromMemory(Memory memory, unsigned long int address)
{
    if (address > MAX_MEMORY_SIZE) // ? negative numbers ?
    {
        return -1;
    }
    else if (address > memory.currentAddress)
    {
        return 0;
    }
    else
    {
        return memory.space[address];
    }
}

int loadDataToMemory(Memory* memory, long int data, unsigned long int address)
{
    if (address > MAX_MEMORY_SIZE)
    {
        error(OUT_OF_BOUNDS_ADDRESS);
        return 0;
    }
    else if (address > memory->currentAddress)
    {
        int i;
        Memory *newMem = malloc(sizeof(Memory));
        newMem->space = createMemory(address+1);
        for (i = 0; i <= memory->currentAddress; i++)
        {
            if (memory->space[i] != 0) newMem->space[i] = memory->space[i];
        }
        memory->space = newMem->space;
        memory->currentAddress = address;
        memory->space[address] = data;
        free(newMem);
        return 1;

        /*REALLOC*/
//        memory->space = (long int*) realloc(memory->space, adress + 1);
//        if (memory->space == NULL)
//        {
//            *error[MEMORY_ALLOCATION_ERROR] = 1;
//            return 0;
//        }
//        memory->space =  memset(memory->space + (sizeof(long int) * (memory->currentAdress)), 0, adress - memory->currentAdress + 1);
//        memory->currentAdress = adress;
//        memory->space[adress] = data;
//        return 1;

    }
    else if (address <= memory->currentAddress)
    {
        memory->space[address] = data;
        return 1;
    }
    return 0;
}

void getStateOfMemory(Memory memory)
{
    int i = 0;
    printf("\nMemory contains:\n");
    for (i = memory.currentAddress; i >= 0; i--)
    {
        printf("%ld at [%d]\n", memory.space[i], i);
    }

    printf("\n");
    // TODO show percentages
}
