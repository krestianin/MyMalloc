#include "my_malloc.h"
#include "stdio.h"
#include <stdlib.h>

int main(){
    printf("\n");
    
    initializeMemory();

    // Example usage
    char* block1 = (char*)myMalloc(100);
    int* block2 = (int*)myMalloc(200);

    *block1 = 'd';
    *block2 = 12;
    puts(block1);
    printf("%d Hello is it me?",*block2);

    printf("\n Somethign");

    // Remember to free in LIFO order
    myFree(block2);
    myFree(block1);

    // Clean up
    cleanupMemory(); 
    return 0;
}