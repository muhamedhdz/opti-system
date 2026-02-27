#pragma once
#include <stdio.h>

typedef struct dataSet {
    int numberOfObjects;
    int capacity;
    int *objectValues;
    int *objectWeights;
} dataSet;

int read_TP1_instance(FILE *fin, dataSet *dataset);

float *knapsackGreedy(const dataSet *dataset);

float *knapsackLinearRelaxation(const dataSet *dataset);

void sort(const dataSet *dataset);

float *initializeProportions(int numberOfObjects);
