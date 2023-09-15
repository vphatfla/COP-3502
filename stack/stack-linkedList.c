#include <stdio.h>
#include <stdlib.h>

typedef struct node
{
    int value;
    node *next;
} node;

typedef struct stack
{
    node *front;
    int size;
} stack;

node pop(stack *st);

int main()
{
    return 0;
}