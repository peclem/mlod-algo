#include <stdio.h>
#include <stdlib.h>


int* repeat(int* t1, int* t2)
{
    int total = 0;
    int sizet1 = sizeof(t1)/sizeof(t1[0]);
    for(int i = 0; i < sizet1 ;i++){
        total += t1[i];
    }
    int* tab;
    tab = malloc(total * sizeof(int));
    int k = 0;
    for(int i = 0; i < sizet1; i++){
        for(int j = 0; j < t1[i]; j++){
            tab[k] = t2[j];
            k++;
        }
    }

    return tab;
}



int main()
{
    int t1[3] = {1,2,4};
    int t2[3] = {10,3,8};
    println(repeat(t1,t2));
}