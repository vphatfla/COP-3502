/*
Van Phat Phan
COP 3502 - section 4
08/24/2023
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXSIZE 100000
#define NUMBERSOFCHARACTER 26

int *getFreq(char *name);

int main()
{

    // init the oldname and newname
    char *oldName = calloc(MAXSIZE + 2, sizeof(char));
    char *newName = calloc(MAXSIZE + 2, sizeof(char));
    // get input from standard input
    fgets(oldName, MAXSIZE + 1, stdin);
    fgets(newName, MAXSIZE + 1, stdin);

    // init the frequeney array for both names
    // 26 stand for 26 chars from A-Z, init all value to 0
    int *freqOld = getFreq(oldName);
    int *freqNew = getFreq(newName);

    // init count
    int countReplacements = 0;
    // run time constant 0(1) = 26
    // only accumulate if freNew[i] > freqOld[i]
    for (int i = 0; i < 26; i += 1)
    {
        countReplacements += (freqNew[i] > freqOld[i]) ? (freqNew[i] - freqOld[i]) : 0;
    }

    // out put
    printf("%d\n", countReplacements);

    // free the memory
    free(oldName);
    free(newName);
    free(freqOld);
    free(freqNew);

    return 0;
}

// to return frequency
int *getFreq(char *name)
{
    // init
    int *freq = calloc(NUMBERSOFCHARACTER, sizeof(int));
    // count the old and new, run time is linear n+m
    // if the char is space, ignore
    for (int i = 0; i < strlen(name); i += 1)
    {
        if (name[i] == ' ')
            continue;
        freq[name[i] - 'A'] += 1;
    }

    return freq;
}