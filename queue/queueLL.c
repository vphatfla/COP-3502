#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{
    int data;
    Node *next;
} Node;

typedef struct queue
{
    Node *front;
    Node *end;
    int size;
} queue;
