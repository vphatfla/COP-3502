/*
Van Phat Phan
COP 3502 Section 4
10/02/2023
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int numberOfPeople;

int check = 0;

int countNumberOfPermuatation;

typedef struct person
{
    char *name;
    // 1 is yes, 0 is no
    int popcorn;
    // list of must not sit near by people
    struct person **listPeopleNotSitNext;
    // size of the list above
    int currSize;
    // int max size of the list above
    int maxSize;
} person;

// create a new person
person *createNewPerson(char *name, int popcorn);

// add a new person to the list of people that thisPerson can't sit next to
void addListPeopleNotSitNext(person *thisPerson, person *thatPerson);

// swamp two person memory address
void swap(person **person1, person **person2);

// permuatation
void permute(person **listPeople, int start, int end);

// check permuatation for proper seating, if good seating return 1, bad seating return 0
int checkPermuatedSeating(person **listPeople);

int main()
{
    // init
    countNumberOfPermuatation = 0;

    setbuf(stdout, NULL);
    int p;
    scanf("%d %d", &numberOfPeople, &p);
    // init list of person
    person **listPeople = calloc(numberOfPeople, sizeof(person *));

    // get input list of person and pop corn status
    for (int i = 0; i < numberOfPeople; i += 1)
    {
        char *name = malloc(50 * sizeof(char));
        int popcorn;
        scanf("%s %d", name, &popcorn);

        person *newPerson = createNewPerson(name, popcorn);
        // add to the listpeople
        listPeople[i] = newPerson;
    }

    for (int i = 0; i < p; i += 1)
    {
        char *name1 = calloc(50, sizeof(char)), *name2 = calloc(50, sizeof(char));
        scanf("%s %s", name1, name2);
        // printf("name1 = %s and name2 = %s\n", name1, name2);
        person *person1, *person2;

        for (int i = 0; i < numberOfPeople; i += 1)
        {
            if (strcmp(listPeople[i]->name, name1) == 0)
                person1 = listPeople[i];
            if (strcmp(listPeople[i]->name, name2) == 0)
                person2 = listPeople[i];
        }

        addListPeopleNotSitNext(person1, person2);
    }

    // start the permuation HERE
    permute(listPeople, 0, numberOfPeople - 1);

    // free the memory
    for (int i = 0; i < numberOfPeople; i += 1)
    {
        free(listPeople[i]->listPeopleNotSitNext);
        free(listPeople[i]->name);
        free(listPeople[i]);
    }
    free(listPeople);
}

person *createNewPerson(char *name, int popcorn)
{
    person *newPerson = malloc(sizeof(person));
    newPerson->name = name;
    newPerson->maxSize = numberOfPeople;
    newPerson->popcorn = popcorn;
    newPerson->currSize = 0;
    newPerson->listPeopleNotSitNext = calloc(newPerson->maxSize, sizeof(person *));
    return newPerson;
}

void addListPeopleNotSitNext(person *thisPerson, person *thatPerson)
{
    thisPerson->listPeopleNotSitNext[thisPerson->currSize] = thatPerson;
    thisPerson->currSize += 1;

    thatPerson->listPeopleNotSitNext[thatPerson->currSize] = thisPerson;
    thatPerson->currSize += 1;
}

void swap(person **person1, person **person2)
{
    // contain the memory address that person1 pointer is pointing to (aka its context)
    person *temp = *person1;
    // swap them
    *person1 = *person2;
    *person2 = temp;
}

void permute(person **listPeople, int start, int end)
{
    // check is true, meaning we already print out the first output
    if (check != 0) return;
    if (start == end)
    {
        if (checkPermuatedSeating(listPeople) == 1)
        {   
            // print out
            for (int i = 0; i < numberOfPeople; i += 1)
            {
                printf("%s\n", listPeople[i]->name);
            }
            check =1;
            return;
        }
    }
    else
    {
        for (int i = start; i <= end; i += 1)
        {
            swap(&listPeople[start], &listPeople[i]);

            // permuate
            permute(listPeople, start + 1, end);

            // reverse the swap
            swap(&listPeople[start], &listPeople[i]);
        }
    }
}

int checkPermuatedSeating(person **listPeople)
{
    for (int i = 0; i < numberOfPeople; i += 1)
    {
        person *currPerson = listPeople[i];

        // person to the left and right
        person *leftPerson = NULL;
        if (i > 0)
            leftPerson = listPeople[i - 1];
        person *rightPerson = NULL;
        if (i < numberOfPeople - 1)
            rightPerson = listPeople[i + 1];

        // check if they can sit next together
        if (leftPerson != NULL && rightPerson != NULL)
        {
            for (int i = 0; i < currPerson->currSize; i += 1)
            {
                // printf("For %s not sitting next to %s\n", currPerson->name, currPerson->listPeopleNotSitNext[i]->name);
                if (leftPerson != NULL && leftPerson == currPerson->listPeopleNotSitNext[i])
                    return 0;
                if (rightPerson != NULL && rightPerson == currPerson->listPeopleNotSitNext[i])
                    return 0;
            }
        }
        // check if they have popcorn
        // only check if the currPerson doesn't have popcorn
        if (currPerson->popcorn != 1)
        {
            // total popcorn of the left + right must be 1 in order to pass
            // if curr is left most the leftPopCorn is 0
            // if curr is right most the rightPopCorn is 0
            int leftPopcorn = 0, rightPopcorn = 0;
            if (i > 0)
                leftPopcorn = leftPerson->popcorn;
            if (i < numberOfPeople - 1)
                rightPopcorn = rightPerson->popcorn;

            if (leftPopcorn + rightPopcorn == 0)
                return 0;
        }
    }

    // passed all test
    return 1;
}

