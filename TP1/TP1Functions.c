#include "TP1Functions.h"
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/time.h>
#include<stdio.h>

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

    fprintf(stderr, "\nInstance file read, we have capacity %d and there is %d items of values/weights:\n",
            capacity, numberOfObjects);
    for (i = 0; i < numberOfObjects; i++)
        fprintf(stderr, "%d,%d\n", dsptr->objectValues[i], dsptr->objectWeights[i]);
    fprintf(stderr, "\n");


    return rval;
}

int KP_greedy(dataSet *dsptr) {
    int rval = 0;


    return rval;
}

int KP_LP(dataSet *dsptr) {
    int rval = 0;

    sort(dsptr);


    return rval;
}

void sort(dataSet *dsptr) {
    for (int i = 0; i < dsptr->numberOfObjects; i++) {
        int value = dsptr->objectValues[i];
        int weight = dsptr->objectWeights[i];

        float currentValue = (float) value / (float) weight;

        for (int j = 0; j < dsptr->numberOfObjects; j++) {
            int valueToCompare = dsptr->objectValues[j];
            int weightToCompare = dsptr->objectWeights[j];

            float currentValueToCompare = (float) valueToCompare / (float) weightToCompare;

            if (currentValueToCompare > currentValue) {
                int tempValue = value;
                int tempWeight = weight;

                dsptr->objectWeights[j] = weight;
                dsptr->objectValues[j] = value;

                dsptr->objectValues[i] = tempValue;
                dsptr->objectWeights[i] = tempWeight;
            }

            break;
        }
    }
}
