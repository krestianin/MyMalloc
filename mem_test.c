#include "my_malloc.h"
#include "stdio.h"
#include <stdlib.h>

int main(){
    printf("\n");
    
    initializeMemory();

    // Example usage
    char* block1 = (char*)myMalloc(4);
    long* block2 = (long*)myMalloc(8);
    long* block3= (long*)myMalloc(2);

    *block1 = 'd';
    *block2 = 100000000000000;
    *block3 = 5;
    puts(block1);
    printf("%lu\n", *block2);
    printf("%lu\n", *block3);

    // Remember to free in LIFO order
    myFree(block3);
    myFree(block2);
    myFree(block1);


    // Clean up
    cleanupMemory(); 
    return 0;
}