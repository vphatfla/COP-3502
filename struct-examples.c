#include <stdio.h>
#include <string.h>
typedef struct name
{
    char *field;
    /* data */
} name;

int main()
{
    int n = 0;
    name *list = malloc(n * sizeof(name));
    // array list of name, each element has size of struct
    // to access attribute
    list[0].field;
    // have to free the memory afterward
    free(list);
    return 0;

    // // pro
    // easy to maintain
    // //con
    // swap might be time consuming.

    // alternative design?
    // array of ptr to struct
    name **list = calloc(n, sizeof(name *));
    // return a array, each element is a POINTER
    // now allocated memory for each element
    for (int i = 0; i < n; i += 1)
    {
        name *list[i] = malloc(sizeof(name *));
    }

    // to swap
    name *temp = list[i];
    list[i] = list[i + 1];
    list[i] = temp;

    // to access
    list[1]->field = 7;
}