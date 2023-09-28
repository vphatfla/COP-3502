#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
typedef struct Island
{
    int val;
    struct Island **adj;
    int currSize;
    int currMaxSize;
    bool visited;
} Island;

Island *createNewIsland(int val, int n);
void addAdjIsland(Island *thisIsland, Island *thatIsland);
void dfs(Island *thisIsland);

int main()
{
    int n, m;
    scanf("%d %d", &n, &m);

    // each of the island has an list of neighbord island
    Island **listIsland = malloc(n * sizeof(Island *));

    for (int i = 0; i < m; i += 1)
    {
        int n1, n2;
        scanf("%d %d", &n1, &n2);
        n1 -= 1;
        n2 -= 1;
        if (listIsland[n1] == NULL)
            listIsland[n1] = createNewIsland(n1, n);

        if (listIsland[n2] == NULL)
            listIsland[n2] = createNewIsland(n2, n);

        // add each to the adj list
        addAdjIsland(listIsland[n1], listIsland[n2]);
        addAdjIsland(listIsland[n2], listIsland[n1]);
    }

    dfs(listIsland[0]);

    int c = 0;
    for (int i = 0; i < n; i += 1)
        if (listIsland[i] == NULL || listIsland[i]->visited == false)
        {
            printf("NO");
            // printf("%d \n", i);
            c = 1;
            break;
            ;
        }

    if (c == 0)
        printf("YES");

    for (int i = 0; i < n; i += 1)
    {
        free(listIsland[i]->adj);
        free(listIsland[i]);
    }
    return 0;
}

Island *createNewIsland(int val, int n)
{
    Island *newIsland = malloc(sizeof(Island));
    newIsland->val = val;
    newIsland->adj = malloc(n * sizeof(Island *));
    newIsland->currSize = 0;
    newIsland->currMaxSize = n;
    newIsland->visited = false;
    return newIsland;
}

void addAdjIsland(Island *thisIsland, Island *thatIsland)
{
    thisIsland->adj[thisIsland->currSize] = thatIsland;
    thisIsland->currSize += 1;
    if (thisIsland->currSize == thisIsland->currMaxSize)
    {
        thisIsland->adj = realloc(thisIsland->adj, thisIsland->currMaxSize * 2);
        thisIsland->currMaxSize *= 2;
    }
}

void dfs(Island *thisIsland)
{
    if (thisIsland->visited == true)
        return;
    thisIsland->visited = true;
    for (int i = 0; i < thisIsland->currSize; i += 1)
        dfs(thisIsland->adj[i]);
}