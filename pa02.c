/*=============================================================================
| Assignment: pa02 - Calculating an 8, 16, or 32 bit
| checksum on an ASCII input file
|
| Author: Van Phat Phan
| Language: C
|
| To Compile: gcc pa02.c -o pa02
|
| To Execute: c -> ./pa02 inputFile.txt 8

| where inputFile.txt is an ASCII input file
| and the number 8 could also be 16 or 32
| which are the valid checksum sizes, all
| other values are rejected with an error message
| and program termination
|
| Note: All input files are simple 8 bit ASCII input
|
| Class: CIS3360 - Security in Computing - Fall 2023
| Instructor: McAlpin
| Due Date: per assignment
|
+=============================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned int long calculateChecksum(char *data, int dataSize)
{
    int index = 0;
    unsigned int long resultMask;
    resultMask = data[index];

    for (dataSize -= 8; dataSize != 0; dataSize -= 8)
    {
        resultMask = (resultMask << 8) + data[index + 1];
        index++;
    }

    return resultMask;
}

void updateChecksum(int dataSize, char *inputData, unsigned int long *checksum)
{
    unsigned int long dataMask;
    dataMask = calculateChecksum(inputData, dataSize);

    *checksum = dataMask + *checksum;

    *checksum = *checksum << (64 - dataSize);
    *checksum = *checksum >> (64 - dataSize);

    // printf(" %s %lx %lx\n", inputData, *checksum, dataMask);
    printf("%s", inputData);
}

void readAndProcessFile(char *filename, int dataSize)
{
    FILE *filePointer;
    int buffer, i = 0, j = 0, k, characterCount = 0, dataWordLength = dataSize / 8 + 1;
    unsigned int long *checksum = calloc(1, sizeof(unsigned int long));
    char *dataOutput = malloc(sizeof(char) * dataWordLength);

    if (dataSize != 8 && dataSize != 16 && dataSize != 32)
    {
        fprintf(stderr, "Valid checksum sizes are 8, 16, or 32\n");
        return;
    }

    filePointer = fopen(filename, "r");
    printf("\n");

    if (filePointer == NULL)
    {
        printf("ERROR: FILE UNABLE TO OPEN\n");
        return;
    }

    while (buffer != EOF)
    {
        if (dataWordLength - i == 1)
        {
            dataOutput[i] = '\0';
            updateChecksum(dataSize, dataOutput, checksum);

            j++;
            i = 0;

            // Print 80 characters per line
            if (j * (dataWordLength - 1) == 80)
                printf("\n");
        }
        else
        {
            buffer = fgetc(filePointer);

            // printf("\n buf %c buf ", buffer);

            if (buffer == EOF)
            {
                if (dataWordLength - i != 1 && dataWordLength - i != dataWordLength)
                {
                    for (k = 0; dataWordLength - i != 1 && dataWordLength - i != dataWordLength; i++, k++)
                        dataOutput[i] = 'X';

                    dataOutput[i] = '\0';

                    updateChecksum(dataSize, dataOutput, checksum);

                    j++;
                    characterCount += k - 1;
                    i = 0;

                    // Print 80 characters per line
                    if (j * (dataWordLength - 1) == 80)
                        printf("\n");
                }
                else
                    break;
            }
            else
                dataOutput[i] = buffer;

            characterCount++;
            i++;
            // printf("\n out %s out %d\n", dataOutput, i);
        }
    }

    printf("\n");
    printf("%2d bit checksum is %8lx for all %4d chars\n", dataSize, *checksum, characterCount);

    fclose(filePointer);
}

int main(int argc, char *argv[])
{
    readAndProcessFile(argv[1], atoi(argv[2]));

    return 0;
}

/*=============================================================================
| I Van Phat Phan (va559212) affirm that this program is
| entirely my own work and that I have neither developed my code together with
| any another person, nor copied any code from any other person, nor permitted
| my code to be copied or otherwise used by any other person, nor have I
| copied, modified, or otherwise used programs created by others. I acknowledge
| that any violation of the above terms will be treated as academic dishonesty.
+============================================================================*/
