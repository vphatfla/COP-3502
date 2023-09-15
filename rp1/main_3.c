#include <stdio.h>
#include <stdlib.h>

int main(void)
{
  int cases, rows, col;

  scanf("%d", &cases);

  for (int i = 1; i < cases + 1; i++)
  {
    scanf("%d%d", &rows, &col);

    char **row = malloc(sizeof(char *) * rows);
    for (int r = 0; r < rows; r++)
      row[r] = calloc((col + 1), sizeof(char));

    for (int n = 0; n < rows; n++)
      for (int j = 0; j < col; j++)
        scanf(" %c", &row[rows - 1 - n][col - 1 - j]);

    printf("Test %d\n", i);
    for (int t = 0; t < rows; t++)
    {
      for (int j = 0; j < col; j++)
        printf("%c", row[t][j]);
      printf("\n");
    }
    for (int k = 0; k < rows; k++)
      free(row[k]);
    free(row);
  }

  return 0;
}