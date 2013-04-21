#include "IOSystem.h"
#include "stack.h"
#include "global.h"

int readString(FILE **input, char* string, int *errors)
{
    int k = 0;
    int charCount = SUPPLIER;
    char ch;

    while (((ch = fgetc(*input)) != '\n') && !feof(*input))
    {
        if (k == charCount)
        {
            string = realloc(string, charCount += SUPPLIER);
            if (string == NULL)
            {
                *errors = MEMORY_ALLOCATION_ERROR;
                return 0;
            }
        }
        string[k++] = ch;
    }
    string[k] = 0;

    if (feof(*input))
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

int findSubstr(char s[], char p[]) //KMP
{
    int i, j, N, M;
    N = strlen(s);
    M = strlen(p);
    int *d =(int*) malloc(M * sizeof(int));

    d[0] = 0;
        for (i=1, j=0; i < M; i++)
        {
            while ( j > 0 && p[j] != p[i])
                j = d[j-1];
            if (p[j] == p[i])
                j++;
            d[i] = j;
        }
    for(i=0, j=0; i < N; i++)
    {
        while(j>0 && p[j]!= s[i])
            j=d[j-1];
        if(p[j] == s[i])
            j++;
        if (j == M)
        {
            free(d);
            return i-j+1;
        }
    }
    free(d);
    return -1;
}

int openFile(FILE **input, const char *fileName)
{
    *input = (FILE*) fopen(fileName, "r");
    if (*input == NULL)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

void checkConfig(Config *config, int* errors)
{
    int pointer, j, sizeChanged = 0, size = 0;
    char* string;

    string = (char*) malloc(sizeof(char*) * SUPPLIER);
    if (string == NULL)
    {
        *errors = MEMORY_ALLOCATION_ERROR;
        return;
    }

    if (!openFile(&config->file, NAME_OF_CONFIG))
    {
        config->memorySize = DEF_MEMORY_SIZE;
        free(string);
        return;
    }

    j = readString(&config->file, string, errors);
    pointer = findSubstr(string, "MemorySize");

    while ((pointer == -1) && j)
    {
        j = readString(&config->file, string, errors);
        pointer = findSubstr(string, "MemorySize");
    }

    if (pointer == -1)
    {
        config->memorySize = DEF_MEMORY_SIZE;
        free(string);
        fclose(config->file);
        return;
    }

    pointer += 10;
    while (string[pointer] != '\0')
    {
        if (string[pointer] >= '0' && string[pointer] <= '9')
        {
            size = 10 * size + (string[pointer] - '0');
            sizeChanged = 1;
        }
        pointer++;
    }

    if (!sizeChanged)
    {
        config->memorySize = DEF_MEMORY_SIZE;
    }
    else
    {
        config->memorySize = size;
    }

    free(string);
    fclose(config->file);
}

int checkArgs(int argc, char *argv[], char *mode)
{
    char *argv1 = argv[1];
    if(argc == 1)
    {
        fprintf(stderr, "Error: Filename not specified.\n");
        return 1;
    }
    else if (argc == 2)
    {
        argv1 = strstr(argv1, ".");
        if(!strcmp(argv1, ".svm")) return 0;
        fprintf(stderr, "Error: Incorrect file format.\n");
        return 1;
    }
    else if (argc == 3)
    {
        argv1 = strstr(argv1, ".");
        if(strcmp(argv1, ".svm")) fprintf(stderr, "Error: Incorrect file format.\n");
        if (strcmp(argv[2], "-sm") == 0) {  *mode = 1; return 0; }
        else fprintf(stderr, "Error: Incorrect parameters.\n");
        return 1;
    }
    return 1;
}

void showStack(Stack *s)
{
    printf("\n-->Stack:\n");
    while(s != NULL)
    {
        printf("> %ld \n", pop(&s));
    }
    printf("\n");
}
