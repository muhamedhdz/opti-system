#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "TP1Functions.h"

int main(int argc, char **argv) {
    char instance_file[1024];
    snprintf(instance_file, sizeof(instance_file), "%s", "instance1.csv");

    int opt;
    while ((opt = getopt(argc, argv, "F:h")) != EOF) {
        switch (opt) {
            case 'F':
                snprintf(instance_file, sizeof(instance_file), "%s", optarg);
                break;
            case 'h':
                fprintf(stderr, "Usage: ./TP1 -F <instance.csv>\n");
                return 0;
            default:
                return 1;
        }
    }

    dataSet data = {0};

    FILE *fin = fopen(instance_file, "r");
    if (!fin) {
        perror("fopen");
        return 1;
    }

    int rval = read_TP1_instance(fin, &data);
    fclose(fin);

    if (rval != 0) {
        fprintf(stderr, "Erreur lecture instance (code %d)\n", rval);
        free(data.objectValues);
        free(data.objectWeights);
        return 1;
    }

    float *xg = KP_greedy(&data);
    free(xg);

    float *xlp = KP_LP(&data);
    free(xlp);

    free(data.objectValues);
    free(data.objectWeights);

    return 0;
}