#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <math.h>
#include <errno.h>
#include <time.h>
#include <assert.h>


typedef struct dataSet {
    int numberOfObjects;
    int capacity;
    int *objectValues;
    int *objectWeights;
} dataSet;

int read_TP1_instance(FILE *fin, dataSet *dsptr);

int KP_greedy(dataSet *dsptr);

int KP_LP(dataSet *dsptr);

void sort(dataSet *dsptr);
