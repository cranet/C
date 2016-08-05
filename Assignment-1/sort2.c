#include "sort2.h"
 
 /*Selection sort.*/
void sort2(int *arr, int n)
{
    int i;
    for(i = 0; i < n; i++)
    {
        int small = arr[i];
        int pos = i;
        int j;
        for(j = i + 1; j < n; j++)
        {
            if(arr[j] < small)
            {
                small = arr[j];
                pos = j;
            }
        }
        int temp = arr[pos];
        arr[pos] = arr[i];
        arr[i] = temp;
    }

}
