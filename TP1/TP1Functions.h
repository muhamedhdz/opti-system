#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <math.h>
#include <errno.h>
#include <time.h>
#include <assert.h>


typedef struct dataSet {
    int numberOfObjects;    //n
    int capacity;           //b
    int *objectValues;      //c
    int *objectWeights;     //a
} dataSet;

int read_TP1_instance(FILE *fin, dataSet *dsptr);

float* KP_greedy(dataSet *dsptr);

float* KP_LP(dataSet *dsptr);

void sort_by_utility(dataSet *dsptr);
