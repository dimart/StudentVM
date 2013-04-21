#include "stack.h"
#include "global.h"

Stack* initStack(void)
{
    Stack *head = NULL;
    return head;
}

void push(Stack **s, long int value)
{
    Stack *node = malloc(sizeof(Stack));

    if(node == NULL)
    {
        //error(MEMORY_ALLOCATION_ERROR);
    }
    else
    {
        node->data = value;
        node->next = *s;
        *s = node;
    }
}

long int pop(Stack **s)
{
    Stack *p = *s;
    long int result = (*s)->data;
    *s = (*s)->next;
    free(p);
    return result;
}

int isEmpty(Stack *s)
{
    return (s == NULL);
}

void terminateStack(Stack **s)
{
    while(!isEmpty(*s))
        pop(s);
}

void swap(Stack **s)
{
    int temp = pop(s);
    int temp2 = pop(s);
    push(s, temp);
    push(s, temp2);
}

int stackSize(Stack *s)
{
    int count = 0;
    while (s != NULL)
    {
        count++;
        s = s->next;
    }
    return count;
}
