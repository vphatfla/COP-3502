#include <stdio.h>

typedef struct node
{
    int data;
    struct node *next;
} node;
node *addValue3(node *head)
{
    if (head->data > 3 || head == NULL)
    {
        node *tmp = malloc(sizeof(node));
        tmp->data = 3;
        tmp->next = head;
        return tmp;
    }
    if (head->data == 3)
        return head;
    node *iter = head;
    while (iter->next != NULL && iter->next->data < 3)
        iter = iter->next;
    if (iter->data == 3 && iter->next->data == 3)
        return head;
    node *tmp = malloc(sizeof(node));
    tmp->data = 3;
    tmp->next = iter->next;
    iter->next = tmp;
    return tmp;
}