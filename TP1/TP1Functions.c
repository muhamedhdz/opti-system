#include "TP1Functions.h"
#include <stdlib.h>
#include <sys/time.h>
#include <stdio.h>

#ifndef DEBUG_READ
#define DEBUG_READ 0
#endif

/* Tri par ratio via indices */
typedef struct {
    int idx;        // index original de l'objet
    double ratio;   // c/a
} RatioIdx;

static int compare_ratio_desc(const void *p1, const void *p2) {
    const RatioIdx *i1 = (const RatioIdx *)p1;
    const RatioIdx *i2 = (const RatioIdx *)p2;

    if (i1->ratio < i2->ratio) return 1;   // décroissant
    if (i1->ratio > i2->ratio) return -1;

    // tie-break stable (optionnel)
    if (i1->idx < i2->idx) return -1;
    if (i1->idx > i2->idx) return 1;
    return 0;
}

/**
 * Construit order[0..n-1] : indices des objets triés par ratio décroissant.
 * Retourne 0 si OK, <0 sinon.
 */
static int build_sorted_order(const dataSet *dsptr, int *order) {
    int n = dsptr->numberOfObjects;

    RatioIdx *tmp = (RatioIdx *)malloc(sizeof(RatioIdx) * n);
    if (!tmp) return -1;

    for (int i = 0; i < n; i++) {
        tmp[i].idx = i;
        tmp[i].ratio = (double)dsptr->objectValues[i] / (double)dsptr->objectWeights[i];
    }

    qsort(tmp, n, sizeof(RatioIdx), compare_ratio_desc);

    for (int i = 0; i < n; i++) {
        order[i] = tmp[i].idx;
    }

    free(tmp);
    return 0;
}

int read_TP1_instance(FILE *fin, dataSet *dsptr) {
    if (!fin || !dsptr) return -1;

    int capacity;
    int numberOfObjects;

    int rval = fscanf(fin, "%d,%d\n", &numberOfObjects, &capacity);
    if (rval != 2 || numberOfObjects <= 0 || capacity < 0) return -2;

    dsptr->capacity = capacity;
    dsptr->numberOfObjects = numberOfObjects;

    dsptr->objectValues  = (int *)malloc(sizeof(int) * numberOfObjects);
    dsptr->objectWeights = (int *)malloc(sizeof(int) * numberOfObjects);

    if (!dsptr->objectValues || !dsptr->objectWeights) {
        free(dsptr->objectValues);
        free(dsptr->objectWeights);
        dsptr->objectValues = NULL;
        dsptr->objectWeights = NULL;
        return -3;
    }

    for (int i = 0; i < numberOfObjects; i++) {
        rval = fscanf(fin, "%d,%d\n", &(dsptr->objectValues[i]), &(dsptr->objectWeights[i]));
        if (rval != 2) {
            free(dsptr->objectValues);
            free(dsptr->objectWeights);
            dsptr->objectValues = NULL;
            dsptr->objectWeights = NULL;
            return -4;
        }
        if (dsptr->objectWeights[i] <= 0) {
            free(dsptr->objectValues);
            free(dsptr->objectWeights);
            dsptr->objectValues = NULL;
            dsptr->objectWeights = NULL;
            return -5;
        }
    }


#if DEBUG_READ
    fprintf(stderr, "\nInstance read: capacity=%d, n=%d\n", capacity, numberOfObjects);
    for (int i = 0; i < numberOfObjects; i++) {
        fprintf(stderr, "c=%d, a=%d\n", dsptr->objectValues[i], dsptr->objectWeights[i]);
    }
    fprintf(stderr, "\n");
#endif

    return 0;
}

float* KP_greedy(dataSet *dsptr) {
    if (!dsptr || !dsptr->objectValues || !dsptr->objectWeights) return NULL;

    int n = dsptr->numberOfObjects;

    int *order = (int *)malloc(sizeof(int) * n);
    if (!order) return NULL;

    // x est dans l'ordre ORIGINAL (index objet du fichier)
    float *x = (float *)calloc((size_t)n, sizeof(float));
    if (!x) {
        free(order);
        return NULL;
    }

    int currentCapacity = dsptr->capacity;
    int z = 0;

    struct timeval start, end;
    gettimeofday(&start, NULL);  // <-- chrono AVANT tri

    if (build_sorted_order(dsptr, order) != 0) {
        free(order);
        free(x);
        return NULL;
    }

    for (int k = 0; k < n; k++) {
        if (currentCapacity == 0) break;

        int j = order[k]; // index original de l'objet
        int w = dsptr->objectWeights[j];
        int v = dsptr->objectValues[j];

        if (currentCapacity >= w) {
            x[j] = 1.0f;
            currentCapacity -= w;
            z += v;
        }
    }

    gettimeofday(&end, NULL);   // <-- chrono APRES algo

    double elapsed = (end.tv_sec - start.tv_sec)
                   + (end.tv_usec - start.tv_usec) / 1000000.0;

    printf("=== KP_Greedy ===\n");
    printf("z = %d\n", z);
    printf("capacity_left = %d\n", currentCapacity);
    printf("EXEC_TIME_GREEDY: %e\n\n", elapsed);

    free(order);
    return x;
}


   float* KP_LP(dataSet *dsptr) {
    if (!dsptr || !dsptr->objectValues || !dsptr->objectWeights) return NULL;

    int n = dsptr->numberOfObjects;

    int *order = (int *)malloc(sizeof(int) * n);
    if (!order) return NULL;

    // x est dans l'ordre ORIGINAL (index objet du fichier)
    float *x = (float *)calloc((size_t)n, sizeof(float));
    if (!x) {
        free(order);
        return NULL;
    }

    double currentCapacity = (double)dsptr->capacity;
    double z_bar = 0.0;

    struct timeval start, end;
    gettimeofday(&start, NULL); // <-- chrono AVANT tri

    if (build_sorted_order(dsptr, order) != 0) {
        free(order);
        free(x);
        return NULL;
    }

    for (int k = 0; k < n; k++) {
        if (currentCapacity <= 0.0) break;

        int j = order[k]; // index original
        double w = (double)dsptr->objectWeights[j];
        double v = (double)dsptr->objectValues[j];

        double frac = currentCapacity / w;
        if (frac > 1.0) frac = 1.0;

        x[j] = (float)frac;
        currentCapacity -= frac * w;
        z_bar += frac * v;
    }

    gettimeofday(&end, NULL);  // <-- chrono APRES algo

    double elapsed = (end.tv_sec - start.tv_sec)
                   + (end.tv_usec - start.tv_usec) / 1000000.0;

    printf("=== KP_LP (Relaxation) ===\n");
    printf("z_bar = %.2f\n", z_bar);
    printf("EXEC_TIME_LP: %e\n\n", elapsed);

    free(order);
    return x;
}