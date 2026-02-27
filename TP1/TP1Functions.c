#include "TP1Functions.h"
#include <stdlib.h>
#include <sys/time.h>
#include<stdio.h>

int read_TP1_instance(FILE *fin, dataSet *dataset) {
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

float *knapsackGreedy(const dataSet *dataset) {
    const int numberOfObjects = dataset->numberOfObjects;

    sort(dataset);

    float *proportions = initializeProportions(numberOfObjects);
    int currentCapacity = dataset->capacity;

    for (int j = 0; j < numberOfObjects; j++) {
        if (currentCapacity == 0) {
            return proportions;
        }

        if (currentCapacity >= dataset->objectWeights[j]) {
            proportions[j] = 1;
            currentCapacity -= dataset->objectWeights[j];
        }
    }

    return proportions;
}

float *knapsackLinearRelaxation(const dataSet *dataset) {
    const int numberOfObjects = dataset->numberOfObjects;

    sort(dataset);

    float *proportions = initializeProportions(numberOfObjects);
    float currentCapacity = (float) dataset->capacity;

    for (int j = 0; j < numberOfObjects; j++) {
        if (currentCapacity == 0) {
            return proportions;
        }

        const float weight = (float) dataset->objectWeights[j];
        const float xj = currentCapacity / weight < 1.0f ? currentCapacity / weight : 1.0f;

        proportions[j] = xj;
        currentCapacity -= xj * weight;
    }

    return proportions;
}

float *initializeProportions(const int numberOfObjects) {
    float *proportions = malloc(numberOfObjects * sizeof(float));
    for (int i = 0; i < numberOfObjects; i++) proportions[i] = 0.0f;
    return proportions;
}

void sort(const dataSet *dataset) {
    const int numberOfObjects = dataset->numberOfObjects;

    for (int i = 0; i < numberOfObjects - 1; i++) {
        for (int j = 0; j < numberOfObjects - i - 1; j++) {
            const float ratio1 = (float) dataset->objectValues[j] / (float) dataset->objectWeights[j];
            const float ratio2 = (float) dataset->objectValues[j + 1] / (float) dataset->objectWeights[j + 1];

            if (ratio1 < ratio2) {
                const int tempValue = dataset->objectValues[j];
                dataset->objectValues[j] = dataset->objectValues[j + 1];
                dataset->objectValues[j + 1] = tempValue;

                const int tempWeight = dataset->objectWeights[j];
                dataset->objectWeights[j] = dataset->objectWeights[j + 1];
                dataset->objectWeights[j + 1] = tempWeight;
            }
        }
    }
}
