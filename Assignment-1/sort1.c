#include "sort1.h"

/*Bubble sort.*/
void sort1(int *arr, int n)
{
    int i;
    for (i = 0; i < n; i++) {
        int j;
        for (j = 0; j < n-i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}
