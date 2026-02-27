#include <ctype.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "TP1Functions.h"
#include "TP1.h"

int main(const int argc, char **argv) {
    //File instance name
    //-F option
    char instance_file[1024];
    snprintf(instance_file, 1024, "%s", "instance1.csv");

    char c;
    while ((c = getopt(argc, argv, "F:h")) != EOF) {
        switch (c) {
            case 'F':
                snprintf(instance_file, 1024, "%s", optarg);
                break;
            case 'h':
                fprintf(stderr, "Usage: ./TP1 [options]\nOptions:\n\n");
                fprintf(stderr, "******** INSTANCE DATA ********\n");
                fprintf(stderr, "\t-F Instance file name to load.......................(default %s).\n", instance_file);
                break;
            default:
                exit(0);
        }
    }

    dataSet data;

    //Open the instance file
    FILE *fin = fopen(instance_file, "r");
    read_TP1_instance(fin, &data);
    fclose(fin);

    //execute your solution methods on the instance you just read
    // KP_greedy(&data);
    // KP_LP(&data);

    // testKnapsackLinearRelaxation(&data);
    testKnapsackGreedy(&data);

    return 0;
}

void testKnapsackLinearRelaxation(const dataSet *dataset) {
    float *x = knapsackLinearRelaxation(dataset);

    for (int i = 0; i < dataset->numberOfObjects; i++) fprintf(stderr, "%f\n", x[i]);
    fprintf(stderr, "\n");

    free(x);
}

void testKnapsackGreedy(const dataSet *dataset) {
    float *x = knapsackGreedy(dataset);

    for (int i = 0; i < dataset->numberOfObjects; i++) fprintf(stderr, "%f\n", x[i]);
    fprintf(stderr, "\n");

    free(x);
}
