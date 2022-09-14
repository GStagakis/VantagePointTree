//C function for implementation of QuickSelect
#include <stdio.h>
#include <stdlib.h>

//C function that swaps two float variables
void swap(float *xp, float *yp)
{
    float temp = *xp;
    *xp = *yp;
    *yp = temp;
}

//C function that swaps two float variables
void swapint(int *xp, int *yp)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}
// It considers the last element as pivot
// and moves all smaller element to left of
// it and greater elements to right
int partition(float arr[], int l, int r)
{
    float x = arr[r];
    int i = l;
    for (int j = l; j <= r - 1; j++) {
        if (arr[j] <= x) {
            swap(&arr[i], &arr[j]);
            i++;
        }
    }
    swap(&arr[i], &arr[r]);
    return i;
}
 
// This function returns k'th smallest
// element in arr[l..r] using QuickSort
// based method.  ASSUMPTION: ALL ELEMENTS
// IN ARR[] ARE DISTINCT
float quickselect(float arr[], int l, int r, int k)
{
    // If k is smaller than number of
    // elements in array
    if (k > 0 && k <= r - l + 1) {
 
        // Partition the array around last
        // element and get position of pivot
        // element in sorted array
        int index = partition(arr, l, r);
 
        // If position is same as k
        if (index - l == k - 1)
            return arr[index];
 
        // If position is more, recur
        // for left subarray
        if (index - l > k - 1)
            return quickselect(arr, l, index - 1, k);
 
        // Else recur for right subarray
        return quickselect(arr, index + 1, r,
                            k - index + l - 1);
    }
 
    // If k is more than number of
    // elements in array
    printf("ERROR USING QUICKSELECT!\nINVALID ARGUMENT!");
    exit(0);
}



//MY FUNCTION FOR PARTITION USED TO FIND SMALLER AND BIGGER VALUES THAN MEDIAN
//partitions distances,points and indexes with given pivot
//returns number of smaller or equal values
int mypartition(float *distances, int l, int r, float median, float **points, int d, int *idx)
{
    int i = l;
    for (int j = l; j <= r; j++) {
        if (distances[j-l] <= median) {
            swap(&distances[i-l], &distances[j-l]);
            swapint(&idx[i], &idx[j]);
            for(int k = 0;k < d;k++){
                swap(&points[i][k], &points[j][k]);
            }
            i++;
        }
    }
    return i;
}