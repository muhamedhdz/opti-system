#pragma once
#include <stdio.h>

typedef struct dataSet {
    int numberOfObjects;
    int capacity;
    int *objectValues;
    int *objectWeights;
} dataSet;

int read_TP2_instance(FILE *fin, dataSet *dataset);

int *knapsackDynamicProgramming(const dataSet *dataset);

void printKnapsackResults(int capacity, int numberOfObjects, const int *maxValues, const int *lastAddedItems, const int *proportions);
