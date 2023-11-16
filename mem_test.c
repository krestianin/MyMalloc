#include "my_malloc.h"


int main(){
    printf("Hello world!");
    return 0;

    int * one = myMalloc(4);
    *one = 12;
    printf("%d", *one);
}