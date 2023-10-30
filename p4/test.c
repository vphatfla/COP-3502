#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

// Global variables
int N, A;
double rA;

// Struct for the group of people coordinate
typedef struct Group {
    int x;
    int y;
    int s;
    double angle;
} Group;

// Function prototypes
Group* createNewGroup(int x, int y, int s);
double calculateAngle(int x, int y);
double radiansToDegrees(double radians);
double degreesToRadians(double degrees);
int findMinPeopleInZone(Group** group_list);
double findMaxEmptyAngle(Group** group_list);
void releaseMemory(Group** group_list);

// Merge sort
void Merge(Group** group_list, int start, int mid, int end);
void MergeSort(Group** group_list, int start, int end);

int main() {
    // Input
    scanf("%d %d", &N, &A);
    rA = degreesToRadians(A);

    // Allocate memory for the list of groups
    Group** group_list = malloc((N * 2) * sizeof(Group*));

    // Get input for N groups
    for (int i = 0; i < N; i++) {
        int x, y, s;
        scanf("%d %d %d", &x, &y, &s);
        group_list[i] = createNewGroup(x, y, s);
        group_list[N + i] = createNewGroup(x, y, s);
        group_list[N + i]->angle = (group_list[N + i]->angle + (2 * M_PI));
    }

    // Sort the group_list based on the angle
    MergeSort(group_list, 0, N * 2 - 1);

    int minInDefaultRange = findMinPeopleInZone(group_list);
    printf("%d\n", minInDefaultRange);

    double max = findMaxEmptyAngle(group_list);
    printf("%.4f\n", radiansToDegrees(max));

    releaseMemory(group_list);
    return 0;
}

Group* createNewGroup(int x, int y, int s) {
    Group* newGroup = malloc(sizeof(Group));
    newGroup->x = x;
    newGroup->y = y;
    newGroup->s = s;
    newGroup->angle = calculateAngle(x, y);
    return newGroup;
}

int findMinPeopleInZone(Group** group_list) {
    int minCount = -1;

    for (int left = 0; left < N; left++) {
        int currCount = 0;
        double currAngle = group_list[left]->angle;
        double rangeAngle = currAngle + rA;
        int right = left + 1;
        while (right < (left + N) && group_list[right]->angle < rangeAngle) {
            if (group_list[right]->angle != currAngle) {
                currCount += group_list[right]->s;
            }
            right++;
        }
        if (minCount == -1 || currCount < minCount) {
            minCount = currCount;
        }
    }

    return minCount;
}

double findMaxEmptyAngle(Group** group_list) {
    double max = 0;

    for (int left = 0; left < N; left++) {
        int right = left + 1;
        double currAngle = group_list[left]->angle;
        while (right < (left + N) && group_list[right]->angle == currAngle) {
            right++;
        }
        if (group_list[right]->angle - currAngle > max) {
            max = group_list[right]->angle - currAngle;
        }
    }

    return max;
}

double calculateAngle(int x, int y) {
    return atan2(y, x);
}

double radiansToDegrees(double radians) {
    return radians * (180.0 / M_PI);
}

double degreesToRadians(double degrees) {
    return degrees * (M_PI / 180.0);
}

void releaseMemory(Group** group_list) {
    for (int i = 0; i < N * 2; i++) {
        free(group_list[i]);
    }
    free(group_list);
}

// Merge sort implementation
void Merge(Group** group_list, int start, int mid, int end) {
    Group** temp;
    int length, left, right, newIndex;

    // Allocate memory for the auxiliary array
    length = end - start + 1;
    temp = calloc(length, sizeof(Group*));

    // Keeps track of our index into our auxiliary array.
    newIndex = 0;

    // Here we copy values into our auxiliary array, so long as there are
    // numbers from both lists to copy.
    left = start;
    right = mid;

    while ((left < mid) || (right <= end)) {
        temp[newIndex] = malloc(sizeof(Group));

        if (right > end || (left < mid && group_list[left]->angle < group_list[right]->angle)) {
            temp[newIndex] = group_list[left];
            left++;
            newIndex++;
        } else {
            temp[newIndex] = group_list[right];
            right++;
            newIndex++;
        }
    }

    // Copy to the original array
    for (int i = start; i <= end; i++) {
        group_list[i] = temp[i - start];
    }
    free(temp);
}

void MergeSort(Group** group_list, int start, int end) {
    int mid;

    // If can sort (# elements > 1)
    if (start < end) {
        mid = (start + end) / 2;
        MergeSort(group_list, start, mid);
        MergeSort(group_list, mid + 1, end);
        Merge(group_list, start, mid + 1, end);
    }
}
