/*
Van Phat Phan
COP 3502-section 4
09/07/2023*/

#include <stdio.h>
#include <stdlib.h>

int main()
{
    int t;
    scanf("%i", &t);

    // run loop
    for (int i = 0; i < t; i += 1)
    {
        printf("Test %i\n", (i + 1));

        int r, c;
        scanf("%i %i", &r, &c);

        // dynamic allocate memory for the 2-darray
        char **imageAr = calloc(r, sizeof(char *));
        // get input for the images
        for (int i = 0; i < r; i += 1)
        {
            // dynamic allocate for each row
            imageAr[i] = calloc(c, sizeof(char));
            scanf("%s", imageAr[i]);
        }

        // print the mirror images
        for (int i = r - 1; i >= 0; i -= 1)
        {
            for (int j = c - 1; j >= 0; j -= 1)
            {
                printf("%c", imageAr[i][j]);
            }
            printf("\n");
        }

        // free the memory
        for (int i = 0; i < r; i += 1)
        {
            free(imageAr[i]);
        }
        free(imageAr);
    }
    return 0;
}