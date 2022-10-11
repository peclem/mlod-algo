#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void echangeContenu(int *a, int *b){
    int p  = *a;
    *a = *b;
    *b = p;
}