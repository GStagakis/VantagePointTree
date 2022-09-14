#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "alloc_2d_float.c"
#include "quickselect.c"
#include "prints.c"

typedef struct vpTree{  //struct
    float *vp;
    float md;
    int idx;
    struct vpTree *inner;
    struct vpTree *outer;
} vpTree;

vpTree *new_vpTree(float *vp, int dimension, float md, int idx) {  //constructor
  vpTree* T = malloc(sizeof(vpTree));
  T->vp = (float *)malloc(dimension * sizeof(float));
  for(int i = 0;i < dimension; i++){
      T->vp[i] = vp[i];
  }
  T->md = md;
  T->idx = idx;
  T->inner = NULL;
  T->outer = NULL;
  return T;
}

void traversePreOrder(vpTree *T, int d) {
    if (T != NULL) {
        printfloatarr(d, T->vp);
        traversePreOrder(T->inner, d);
        traversePreOrder(T->outer, d);
    }
}

vpTree *vpt(float **points, int l, int r, int d, int *idx){
    //TO DO...
    int N = r - l + 1;
    if(N == 0){
        vpTree *T = NULL;
        return T;
    }
    else if(N == 2){
        vpTree *T = new_vpTree(points[r], d, 0, idx[r]);
        T->outer = new_vpTree(points[l], d, 0, idx[l]);
        T->inner = NULL;
        return T;
    }
    else{
        float *distances = (float *)malloc(N * sizeof(float));
        for(int i = l;i <= r;i++){
            distances[i - l] = 0;
            for(int j = 0;j < d;j++){
                distances[i - l] += (points[i][j] - points[r][j]) * (points[i][j] - points[r][j]);        //use square instead of actual distance
            }
        }

        float *distances_copy = (float *)malloc(N * sizeof(float));
        for (int i = 0; i < N; i++)    distances_copy[i] = distances[i];    //distances copy to use(and mess up) in quickselect
        float median = quickselect(distances_copy, 0, N-1, (N+1)/2);        //find median distance using quickselect 
        free(distances_copy);
        
        vpTree *T = new_vpTree(points[r], d, median, idx[r]);   //new node

        printdistances(N, distances);
        printindexes(N, idx+l);
        mypartition(distances, l, r, median, points, d, idx);
        printPoints(N, d, points+l);
        printdistances(N, distances);
        printindexes(N, idx+l);
        
        free(distances);
        
        //after mypartition median is in (N+1)/2 - 1 = (N-1)/2 position
        //inner : l -- l + (N-3)/2   outer : l + (N+1)/2 -- r
        T->inner = vpt(points, l, l + (N-3)/2, d, idx);
        T->outer = vpt(points, l + (N+1)/2, r, d, idx);
        return T;
    }
}

int main(int argc, char **argv){
    int N = atoi(argv[1]), d = atoi(argv[2]);     //N:number of total points    d:number of point dimensions

    float **points = alloc_2d_float(N, d);      //allocates Nxd points matrix
    srand(time(NULL));      //Intializes random number generator
    for(int i = 0; i < N; i++){     //Initializes points matrix with random numbers
        for(int j = 0; j < d; j++){
            points[i][j] = rand() % 10 + (rand() % 10)/10.0;
        }
    }
    printPoints(N, d, points);

    int *idx = (int *)malloc(N * sizeof(int));  //indexes array
    for (int i = 0; i < N; i++) idx[i] = i;
    
    vpTree *T;
    T = vpt(points, 0, N-1, d, idx);
    traversePreOrder(T, d);
    
    return 0;
}