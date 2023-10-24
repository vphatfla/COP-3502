/*
Van Phat Phan
COP-3502 Section 4
10/22/2023
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif
// global variables
int N, A, rA;   // N: number of people, A: initial angle dree, rA: angle in radian unit

// struct for the group of people coordinate
typedef struct group
{
    int x;
    int y;
    int s;
    double angle;
} group;

// functions
group* createNewGroup(int x, int y, int s);
void freeMemory(group **list_group);
double calculateAngle(int x, int y); // radian unit, range from -pi to pi
double radiansToDegrees(double radians);
double degreesToRadians(double degrees);
// merge sort
void Merge(group** list_group, int start, int mid, int end);
void MergeSort(group** lsit_group, int start, int end);
// calculate minimum number of peoplein rA
void minimumPeopleInRange(group** list_group);
int main() {
    // take input
    scanf("%d %d", &N, &A);
    rA = degreesToRadians(A);

    // allocate memory for list of group
    group **list_group = malloc((N*2)* sizeof(group*));
    
    // get input for N groups
    for (int i=0; i<N; i+=1) {
        int x,y,s;
        scanf("%d %d %d", &x,&y,&s);
        list_group[i] = createNewGroup(x,y,s);
        list_group[N+i] = createNewGroup(x,y,s);
        list_group[N+i]->angle = (list_group[N+i]->angle + (2*M_PI));  
    }

    // print out group information
    for (int i=0; i<N*2; i+=1) {
        printf("Group %d\n", i);
        printf("x = %d, y = %d, s = %d \n",list_group[i]->x, list_group[i]->y, list_group[i]->s);
        printf("Angle = %f\n", list_group[i]->angle);
    }

    // sort the list_group based on the angle
    MergeSort(list_group, 0, N*2-1);
    printf("AFTER SORT -----------------------------\n\n\n");
    // reprint
    for (int i=0; i<N*2; i+=1) {
        printf("Group %d\n", i);
        printf("x = %d, y = %d, s = %d \n",list_group[i]->x, list_group[i]->y, list_group[i]->s);
        printf("Angle = %f\n", list_group[i]->angle);
    }


    freeMemory(list_group);
    return 0;
}

group* createNewGroup(int x, int y, int s) {
    group* newGroup = malloc(sizeof(group));
    newGroup->x = x;
    newGroup->y = y;
    newGroup->s = s;
    newGroup->angle = calculateAngle(x,y);
    return newGroup;
}
void minimumPeopleInRange(group** list_group) {
    int count =0;
}
double calculateAngle(int x, int y) {
    return atan2(y,x);
}

double radiansToDegrees(double radians) {
    return radians * (180.0 / M_PI);
}

double degreesToRadians(double degrees) {
    return degrees * (M_PI / 180.0);
}

void freeMemory(group **list_group) {
    for (int i=0; i<N*2; i+=1) {
        free(list_group[i]);
    }
    free(list_group);
}


// merge sort implementation
void Merge(group** list_group, int start, int mid, int end) {
    group** temp;
    int length, left, right, newIndex; 
    
    // allocate memory for the auxiliary array
    length = end - start + 1;
    temp = calloc(length, sizeof(group *));

    // Keeps track of our index into our auxiliary array.
    newIndex = 0;
    // Here we copy values into our auxiliary array, so long as there are 
    // numbers from both lists to copy.
    left = start;
    right = mid;
    while ((left < mid) || (right <= end)) { 
        temp[newIndex] = malloc(sizeof(group));
        if (right>end || (left < mid && list_group[left]->angle < list_group[right]->angle)) {
            temp[newIndex] = list_group[left];
            left +=1;
            newIndex += 1;
        }
        // copy the next value from the list two
        else {
            temp[newIndex] = list_group[right];
            right +=1;
            newIndex +=1;
        }
    }

    // copy to the original array
    for (int i= start; i<=end; i+=1) {
        list_group[i] = temp[i-start];
    }
    free(temp);
}
void MergeSort(group** list_group, int start, int end) {
    int mid;
    
    // if can sort (# element > 1)
    if (start < end) {
        mid = (start + end) / 2;

        MergeSort(list_group, start, mid);
        MergeSort(list_group, mid +1, end);
        Merge(list_group, start, mid +1, end);
    }
}