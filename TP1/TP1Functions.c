#include "TP1Functions.h"
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/time.h>
#include <stdio.h>

int read_TP1_instance(FILE *fin, dataSet *dsptr) {
    int rval = 0;

    int capacity;
    int numberOfObjects;

    rval = fscanf(fin, "%d,%d\n", &numberOfObjects, &capacity);
    dsptr->capacity = capacity;
    dsptr->numberOfObjects = numberOfObjects;
    dsptr->objectValues = (int *) malloc(sizeof(int) * numberOfObjects);
    dsptr->objectWeights = (int *) malloc(sizeof(int) * numberOfObjects);


    int i;
    for (i = 0; i < numberOfObjects; i++)
        rval = fscanf(fin, "%d,%d\n", &(dsptr->objectValues[i]), &(dsptr->objectWeights[i]));

    fprintf(stderr, "\nInstance file read, we have capacity %d and there is %d items of values/weights:\n", capacity, numberOfObjects);

    for (i = 0; i < numberOfObjects; i++)
        fprintf(stderr, "%d,%d\n", dsptr->objectValues[i], dsptr->objectWeights[i]);
    fprintf(stderr, "\n");

    return rval;
}

float* KP_greedy(dataSet *dsptr) {
    sort_by_utility(dsptr);

    float *x = (float*)malloc(sizeof(float) * dsptr->numberOfObjects);    // result tab
    for (int i = 0; i < dsptr->numberOfObjects; i++) {
        x[i] = 0.0;
    }

    int currentCapacity = dsptr->capacity;

    for (int i = 0; i < dsptr->numberOfObjects; i++) {
        if (currentCapacity == 0) {
            return x;
        }

        if (currentCapacity >= dsptr->objectWeights[i]) {
            x[i] = 1.0;
            currentCapacity = currentCapacity - dsptr->objectWeights[i];
        }
    }
    return x;
}


float* KP_LP(dataSet *dsptr) {
    sort_by_utility(dsptr);

    float *x = (float*)malloc(sizeof(float) * dsptr->numberOfObjects);    // result tab
    for (int i = 0; i < dsptr->numberOfObjects; i++) {
        x[i] = 0.0;
    }

    int currentCapacity = dsptr->capacity;

    for (int j = 0; j < dsptr->numberOfObjects; j++) {
        if (currentCapacity == 0) {
            return x;
        }

        if (currentCapacity / dsptr->objectWeights[j] < 1.0) {
            x[j] = currentCapacity / dsptr->objectWeights[j];
        } else {
            x[j] = 1.0;
        }
        
        currentCapacity = currentCapacity - x[j] * dsptr->objectWeights[j];
    }

    return x;
}

void sort_by_utility(dataSet *dsptr) {
    for (int i = 0; i < dsptr->numberOfObjects; i++) {
        int value = dsptr->objectValues[i];
        int weight = dsptr->objectWeights[i];

        float currentUtility = (float) value / (float) weight;

        int j = i;
        bool valuesSwitched = false;
        while (!valuesSwitched && j < dsptr->numberOfObjects) {
            int valueToCompare = dsptr->objectValues[j];
            int weightToCompare = dsptr->objectWeights[j];

            float currentUtilityToCompare = (float) valueToCompare / (float) weightToCompare;

            if (currentUtilityToCompare > currentUtility) {
                int tempValue = valueToCompare;
                int tempWeight = weightToCompare;

                // currentUtilityToCompare
                dsptr->objectValues[j] = value;
                dsptr->objectWeights[j] = weight;

                // currentUtility
                dsptr->objectValues[i] = tempValue;
                dsptr->objectWeights[i] = tempWeight;

                valuesSwitched = true;
            }

            j++;
        }

    }
}
