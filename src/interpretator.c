#include "interpretator.h"
#include "exceptionHandler.h"
#include "global.h"
#include "IOSystem.h"

int interpret(VM* SVM, const Code* code)
{
    unsigned int i = 0, j = 0;
    int hlt = 0;
    long int a, b;
    TableOfLabels table;

    table.label = (Label*) calloc(sizeof(Label), code->countOfLabels);
    if (table.label == NULL)
    {
        fprintf(stderr, "Too mach labels.\n");
        error(MEMORY_ALLOCATION_ERROR);
        return 0;
    }

    for (i = 0; i < code->currentCountOfLines; i++)
    {
        if (code->commands[i].opcode == LBL)
        {
            table.label[j].labdef = code->commands[i].labeldef;
            table.label[j++].lineNumber = code->commands[i].labelLine;
        }
    }

    i = 0;
    while ((i <= code->currentCountOfLines) && (hlt != 1))
    {
        switch(code->commands[i].opcode)
        {
        case LDC:
            push(&SVM->stack, code->commands[i].args.number);
            break;
        case LDI:
            if (isEmpty(SVM->stack))
            {
                fprintf(stderr, "Error calling 'LDI' line#%d: ", i+1);
                error(NOT_ENOUGH_ELEMENTS);
                return 0;
            }
            a = pop(&SVM->stack);
            if ((a < 0) && (a > MAX_MEMORY_SIZE))
            {
                error(OUT_OF_BOUNDS_ADDRESS);
                return 0;
            }
            push(&SVM->stack, getDataFromMemory(SVM->memory, a));
            break;
        case LD:
            push(&SVM->stack, getDataFromMemory(SVM->memory, code->commands[i].args.address));
            break;
        case ST:
            if (isEmpty(SVM->stack))
            {
                fprintf(stderr, "Error calling 'ST' line#%d: ", i+1);
                error(NOT_ENOUGH_ELEMENTS);
                return 0;
            }
            else loadDataToMemory(&SVM->memory, pop(&SVM->stack), code->commands[i].args.address);
            break;
        case STI:
            if ((stackSize(SVM->stack) < 2))
            {
                fprintf(stderr, "Error calling 'STI' line#%d: ", i+1);
                error(NOT_ENOUGH_ELEMENTS);
                return 0;
            }
            else
            {
                a = pop(&SVM->stack);
                b = pop(&SVM->stack);
                loadDataToMemory(&SVM->memory, a, b);
            }
            break;
        case ADD:
            if ((stackSize(SVM->stack) < 2))
            {
                fprintf(stderr, "Error calling 'ADD' line#%d: ", i+1);
                error(NOT_ENOUGH_ELEMENTS);
                return 0;
            }
            else
            {
                push(&SVM->stack, pop(&SVM->stack) + pop(&SVM->stack));
                break;
            }
        case SUB:
            if (stackSize(SVM->stack) < 2)
            {
                fprintf(stderr, "Error calling 'SUB' line#%d: ", i+1);
                error(NOT_ENOUGH_ELEMENTS);
                return 0;
            }
            else
            {
                a = pop(&SVM->stack);
                b = pop(&SVM->stack);
                push(&SVM->stack, b - a);
                break;
            }
        case MUL:
            if (stackSize(SVM->stack) < 2)
            {
                fprintf(stderr, "Error calling 'MUL' line#%d: ", i+1);
                error(NOT_ENOUGH_ELEMENTS);
                return 0;
            }
            else
            {
                push(&SVM->stack, pop(&SVM->stack) * pop(&SVM->stack));
                break;
            }
        case DIV:
            if (stackSize(SVM->stack) < 2)
            {
                fprintf(stderr, "Error calling 'DIV' line#%d: ", i+1);
                error(NOT_ENOUGH_ELEMENTS);
                return 0;
            }
            else
            {
                a = pop(&SVM->stack);
                if (a == 0)
                {
                    error(DEVISION_BY_ZERO);
                    return 0;
                }
                b = pop(&SVM->stack);
                push(&SVM->stack, b / a);
                break;
            }
        case MOD:
            if (stackSize(SVM->stack) < 2)
            {
                fprintf(stderr, "Error calling 'MOD' line#%d: ", i+1);
                error(NOT_ENOUGH_ELEMENTS);
                return 0;
            }
            else
            {
                a = pop(&SVM->stack);
                if (a == 0)
                {
                    error(DEVISION_BY_ZERO);
                    return 0;
                }
                b = pop(&SVM->stack);
                push(&SVM->stack, b % a);
                break;
            }
        case CMP:
            if (stackSize(SVM->stack) < 2)
            {
                fprintf(stderr, "Error calling 'CMP' line#%d: ", i+1);
                error(NOT_ENOUGH_ELEMENTS);
                return 0;
            }
            else
            {
                a = pop(&SVM->stack);
                b = pop(&SVM->stack);
                if (a < b) push(&SVM->stack, 1);
                else if (a == b) push(&SVM->stack, 0);
                else if (a > b) push(&SVM->stack, -1);
                break;
            }
        case JMP:
            if (!jmp(code, &table, &i)) return 0;
            break;
        case BR:
            if (isEmpty(SVM->stack))
            {
                fprintf(stderr, "Error calling 'BR' line#%d: ", i+1);
                error(NOT_ENOUGH_ELEMENTS);
                return 0;
            }
            else
            {
                a = pop(&SVM->stack);
                if (a)
                {
                    if (!jmp(code, &table, &i)) return 0;
                }
            }
            break;
        case HLT:
            hlt++;
            break;
        default:
            break;
        }
        i++;
    }
    if (hlt != 1) error(NO_HLT);
    free(table.label);
    return 1;
}

int jmp(const Code *code, const TableOfLabels *table, unsigned int *i)
{
    unsigned int k;
    for (k = 0; k < code->countOfLabels; k++)
    {
        if (strcmp(code->commands[*i].args.label, table->label[k].labdef) == 0)
        {
            *i = table->label[k].lineNumber;
            return 1;
        }
    }
    fprintf(stderr, "Error calling 'JMP' or 'BR' line#%d: ", *i+1);
    fprintf(stderr, "Label \"%s\" does not exist. \n", code->commands[*i].args.label);
    error(NONEXISTENT_LABEL);
    return 0;
}
