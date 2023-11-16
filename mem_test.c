#include "my_malloc.h"
#include "stdio.h"

int main(){
    printf("\n");
    

    initializeMemory();

    int * one = NULL;
    printf("%p", one);
    
    one = (int*)myMalloc(4);
    *one = 12;
    printf("%p", one);
    printf("\n");
    myFree(one);
    printf("%p", one);


    return 0;
}