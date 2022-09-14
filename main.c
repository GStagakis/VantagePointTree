#include <stdio.h>
#include <time.h>
#include "prints.c"
#include "vptree.c"

int main(int argc, char **argv){
    int N = atoi(argv[1]), d = atoi(argv[2]), mode = atoi(argv[3]), THRESHOLD = (argc == 5) ? atoi(argv[4]):150;     // N:number of total points    d:number of point dimensions     mode: 0 -> sequential, 1 -> cilk, 2 -> thresholded, THRESHOLD: default 150

    float **points = alloc_2d_float(N, d);      // allocates Nxd points matrix
    srand(time(NULL));      // Initializes random number generator
    for(int i = 0; i < N; i++){     // Initializes points matrix with random numbers
        for(int j = 0; j < d; j++){
            points[i][j] = rand() % 10 + (rand() % 10)/10.0;
        }
    }
    // printPoints(N, d, points);   // Uncomment to print input points

    int *idx = (int *)malloc(N * sizeof(int));  // indexes array
    for (int i = 0; i < N; i++) idx[i] = i;
    
    vpTree *T;

    // Benchmarking
    struct timespec start, finish;
    double elapsed;

    clock_gettime(CLOCK_MONOTONIC, &start);
    switch(mode){
        case 0:
            T = vpt(points, 0, N-1, d, idx);
            break;
        case 1:
            T = cilk_vpt(points, 0, N-1, d, idx);
            break;
        case 2:
            T = thresh_vpt(points, 0, N-1, d, idx, THRESHOLD);
            break;
        default:
            printf("Wrong mode ...\n");
            return 0;
    }
    clock_gettime(CLOCK_MONOTONIC, &finish);

    elapsed = (finish.tv_sec - start.tv_sec);
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
    // traversePreOrder(T, d);  // Uncomment to get results printed
    printf("%f\n", elapsed);
    return 0;
}