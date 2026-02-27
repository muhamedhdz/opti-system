#include "TP2Functions.h"
#include <sys/time.h>
#include<stdio.h>
#include <stdlib.h>
#define max(a,b) ((a) > (b) ? (a) : (b))

int read_TP2_instance(FILE *fin, dataSet *dataset) {
    int returnedValue = 0;

    int capacity;
    int numberOfObjects;

    returnedValue = fscanf(fin, "%d,%d\n", &numberOfObjects, &capacity);
    dataset->capacity = capacity;
    dataset->numberOfObjects = numberOfObjects;
    dataset->objectValues = (int *) malloc(sizeof(int) * numberOfObjects);
    dataset->objectWeights = (int *) malloc(sizeof(int) * numberOfObjects);

    for (int i = 0; i < numberOfObjects; i++) {
        returnedValue = fscanf(fin, "%d,%d\n", &dataset->objectValues[i], &dataset->objectWeights[i]);
    }

    fprintf(stderr, "\nInstance file read, we have capacity %d and there is %d items of values/weights:\n",
            capacity, numberOfObjects);
    for (int i = 0; i < numberOfObjects; i++) {
        fprintf(stderr, "%d,%d\n", dataset->objectValues[i], dataset->objectWeights[i]);
    }
    fprintf(stderr, "\n");


    return returnedValue;
}

int *knapsackDynamicProgramming(const dataSet *dataset) {
    const int numberOfObjects = dataset->numberOfObjects;
    const int capacity = dataset->capacity;

    int *maxValues = calloc(capacity + 1, sizeof(int));
    int *lastAddedItems = calloc(capacity + 1, sizeof(int));
    int *previousMaxValues = malloc((capacity + 1) * sizeof(int));

    for (int k = 0; k < numberOfObjects; k++) {
        for (int y = 0; y <= capacity; y++) previousMaxValues[y] = maxValues[y];

        const int tempWeight = dataset->objectWeights[k];
        const int tempValue = dataset->objectValues[k];

        for (int y = tempWeight; y <= capacity; y++) {
            if (previousMaxValues[y - tempWeight] + tempValue > previousMaxValues[y]) {
                lastAddedItems[y] = k + 1;
                maxValues[y] = max(previousMaxValues[y], previousMaxValues[y - tempWeight] + tempValue);
            }
        }
    }

    int y = capacity;
    int *proportions = calloc(numberOfObjects, sizeof(int));

    while (y > 0) {
        while (y > 0 && maxValues[y] == maxValues[y - 1]) y--;

        if (y > 0) {
            if (lastAddedItems[y] > 0) {
                proportions[lastAddedItems[y] - 1] = 1;
                y -= dataset->objectWeights[lastAddedItems[y] - 1];
            } else {
                break;
            }
        }
    }

    printKnapsackResults(capacity, numberOfObjects, maxValues, lastAddedItems, proportions);

    free(maxValues);
    free(lastAddedItems);
    free(previousMaxValues);

    return proportions;
}

void printKnapsackResults(
    const int capacity,
    const int numberOfObjects,
    const int *maxValues,
    const int *lastAddedItems,
    const int *proportions
) {
    printf("Z = (");
    for (int i = 0; i <= capacity; i++) printf("%d%s", maxValues[i], i == capacity ? "" : ", ");
    printf(")\n");

    printf("D = (");
    for (int i = 0; i <= capacity; i++) printf("%d%s", lastAddedItems[i], i == capacity ? "" : ", ");
    printf(")\n");

    printf("x* = (");
    for (int i = 0; i < numberOfObjects; i++)printf("%d%s", proportions[i], i == numberOfObjects - 1 ? "" : ", ");
    printf(")\n");
}
