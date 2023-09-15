#include <stdio.h>
#include <stdlib.h>

#define SIZE 10
#define EMPTY -1

typedef struct stack
{
    int item[SIZE];
    int top;
} stack;

void initialize(struct stack *stackPtr);
int full(struct stack *stackPtr);
int push(struct stack *stackPtr, int value);
int empty(struct stack *stackPtr);
int pop(struct stack *stackPtr);
int top(struct stack *stackPtr);

int main()
{
    return 0;
}

void initialize(struct stack *stackPtr)
{
    stackPtr->top = -1;
}

// If the push occurs, 1 is returned. If the
// stack is full and the push can't be done, 0 is
// returned.
int push(struct stack *stackPtr, int value)
{
    // check full
    if (full(stackPtr))
        return 0;
    // add value to the end
    stackPtr->item[stackPtr->top + 1] = value;
    stackPtr->top += 1;
    return 1;
}

int full(struct stack *stackPtr)
{
    return (stackPtr->top == SIZE - 1);
}

int empty(struct stack *stackPtr)
{
    return (stackPtr->top == -1);
}

// Pre-condition: The stack pointed to by stackPtr is NOT empty.
// Post-condition: The value on the top of the stack is popped and returned.
// Note: If the stack pointed to by stackPtr is empty, -1 is returned.
int pop(struct stack *stackPtr)
{
    if (empty(stackPtr))
        return EMPTY;
}
