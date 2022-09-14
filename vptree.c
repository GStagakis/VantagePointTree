#include <stdlib.h>
#include "alloc_2d_float.c"
#include "quickselect.c"
#include <cilk/cilk.h>

typedef struct vpTree {  // struct
    float *vp;
    float md;
    int idx;
    struct vpTree *inner;
    struct vpTree *outer;
} vpTree;

vpTree *new_vpTree(float *vp, int dimension, float md, int idx) {  // constructor
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


// SEQUENTIAL
vpTree *vpt(float **points, int l, int r, int d, int *idx){
    int N = r - l + 1;
    if(N == 0){
        vpTree *T = NULL;
        return T;
    }
    else if(N == 1){
        vpTree *T = new_vpTree(points[r], d, 0, idx[r]);
        return T;
    }
    else{
        float *distances = (float *)malloc((N-1) * sizeof(float));
        for(int i = l;i < r;i++){
            distances[i - l] = 0;
            for(int j = 0;j < d;j++){
                distances[i - l] += (points[i][j] - points[r][j]) * (points[i][j] - points[r][j]);        // use square instead of actual distance
            }
        }

        float *distances_copy = (float *)malloc((N-1) * sizeof(float));
        for (int i = 0; i < N-1; i++)    distances_copy[i] = distances[i];    // distances copy to use(and mess up) in quickselect
        float median = quickselect(distances_copy, 0, N-2, N/2);        // find median distance using quickselect 
        free(distances_copy);
        
        vpTree *T = new_vpTree(points[r], d, median, idx[r]);   // create new node

        mypartition(distances, l, r-1, median, points, d, idx);
        
        free(distances);
        
        // after mypartition median is in r position
        // inner : l -- l + (N-2)/2   outer : l + N/2 -- r-1
        T->inner = vpt(points, l, l + (N-2)/2, d, idx);
        T->outer = vpt(points, l + N/2, r-1, d, idx);
        return T;
    }
}


// CILK
vpTree *cilk_vpt(float **points, int l, int r, int d, int *idx){
    int N = r - l + 1;
    if(N == 0){
        vpTree *T = NULL;
        return T;
    }
    else if(N == 1){
        vpTree *T = new_vpTree(points[r], d, 0, idx[r]);
        return T;
    }
    else{
        float *distances = (float *)malloc((N-1) * sizeof(float));
        cilk_for(int i = l;i < r;i++){
            distances[i - l] = 0;
            for(int j = 0;j < d;j++){
                distances[i - l] += (points[i][j] - points[r][j]) * (points[i][j] - points[r][j]);        // use square instead of actual distance
            }
        }

        float *distances_copy = (float *)malloc((N-1) * sizeof(float));
        cilk_for (int i = 0; i < N-1; i++)    distances_copy[i] = distances[i];    // distances copy to use(and mess up) in quickselect
        float median = quickselect(distances_copy, 0, N-2, N/2);        // find median distance using quickselect 
        free(distances_copy);
        
        vpTree *T = new_vpTree(points[r], d, median, idx[r]);   // create new node

        mypartition(distances, l, r-1, median, points, d, idx);
        
        free(distances);
        
        // after mypartition median is in r position
        // inner : l -- l + (N-2)/2   outer : l + N/2 -- r-1
        T->inner = cilk_spawn cilk_vpt(points, l, l + (N-2)/2, d, idx);
        T->outer = cilk_spawn cilk_vpt(points, l + N/2, r-1, d, idx);
        return T;
    }
}

// THRESHOLDED
vpTree *thresh_vpt(float **points, int l, int r, int d, int *idx, int THRESHOLD){
    int N = r - l + 1;
    if(N == 0){
        vpTree *T = NULL;
        return T;
    }
    else if(N == 1){
        vpTree *T = new_vpTree(points[r], d, 0, idx[r]);
        return T;
    }
    else{
        float *distances = (float *)malloc((N-1) * sizeof(float));
        cilk_for(int i = l;i < r;i++){
            distances[i - l] = 0;
            for(int j = 0;j < d;j++){
                distances[i - l] += (points[i][j] - points[r][j]) * (points[i][j] - points[r][j]);        // use square instead of actual distance
            }
        }

        float *distances_copy = (float *)malloc((N-1) * sizeof(float));
        cilk_for (int i = 0; i < N-1; i++)    distances_copy[i] = distances[i];    // distances copy to use(and mess up) in quickselect
        float median = quickselect(distances_copy, 0, N-2, N/2);        // find median distance using quickselect 
        free(distances_copy);
        
        vpTree *T = new_vpTree(points[r], d, median, idx[r]);   // create new node

        mypartition(distances, l, r-1, median, points, d, idx);
        
        free(distances);
        
        // after mypartition median is in r position
        // inner : l -- l + (N-2)/2   outer : l + N/2 -- r-1
        if(N > THRESHOLD){
            T->inner = cilk_spawn thresh_vpt(points, l, l + (N-2)/2, d, idx, THRESHOLD);
            T->outer = cilk_spawn thresh_vpt(points, l + N/2, r-1, d, idx, THRESHOLD);
        }
        else {
            T->inner = vpt(points, l, l + (N-2)/2, d, idx);
            T->outer = vpt(points, l + N/2, r-1, d, idx);
        }
        return T;
    }
}