#include <stdio.h>
#define MAXSIZE 10

typedef struct claimList
{
    int *claims;
    int maxSize;  // acutual size array claims
    int currSize; // values currently stored
    long long sumCost;
} claimList;

claimList *makeNewClaimList(); // maxsize at 10

// add value to end of array claims
// update currSize
// update sumCost
// if array full, meaning (currSize == maxSize), realloc first
void addClaim(claimList *ptr, int value);

void print(const claimList *prt);

void freeMemory(claimList *ptr);

int main()
{
    claimList *mine = makeNewClaimList;

    return 0;
}

// return a new pointer to a new memory claim list
claimList *makeNewClaimList()
{
    // newItem is a pointer of the claimList struct
    claimList *newItem = malloc(sizeof(claimList));
    newItem->claims = calloc(MAXSIZE, sizeof(int));
    newItem->maxSize = MAXSIZE;
    newItem->currSize = 0;

    return newItem;
}

void freeMemory(claimList *ptr)
{
    free(ptr->claims);
    free(ptr);
}

void addClaim(claimList *ptr, int value)
{
    if (ptr->currSize == ptr->maxSize)
    {
        ptr->maxSize *= 2;
        ptr->claims = realloc(ptr->claims, ptr->maxSize * sizeof(int));
    }

    ptr->claims[ptr->currSize] = value;
    ptr->currSize += 1;
    ptr->sumCost += value;
}

void print(const claimList *ptr)
{
}