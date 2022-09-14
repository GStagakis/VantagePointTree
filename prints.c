//function used to print (numofpoints x d) float 2d array
void printPoints(int numofpoints,  int d, float **points){
    printf("Points:\n");
    for(int i = 0; i < numofpoints; i++){
        for(int j = 0; j < d; j++){
            printf("%.2f ", points[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void printdistances(int numofpoints, float *distances){
    printf("Distances:\n");
    for(int i = 0;i < numofpoints;i++) printf("%.2f ", distances[i]);
    printf("\n");
}

void printindexes(int numofpoints, int *idx){
    for(int i = 0;i < numofpoints;i++) printf("%d ", idx[i]);
    printf("\n");
}

void printfloatarr(int size, float *arr){
    for(int i = 0;i < size;i++) printf("%.2f ", arr[i]);
    printf("\n");
}