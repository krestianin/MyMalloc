// #include "my_malloc.h"
// #include "stdio.h"
// #include <stdlib.h>

// int main(){
//     printf("\n");
    
//     initializeMemory();

//     // Example usage
//     char* block1 = (char*)myMalloc(4);
//     long* block2 = (long*)myMalloc(8);
//     long* block3= (long*)myMalloc(2);

//     *block1 = 'd';
//     *block2 = 100000000000000;
//     *block3 = 5;
//     puts(block1);
//     printf("%lu\n", *block2);
//     printf("%lu\n", *block3);

//     // Remember to free in LIFO order
//     myFree(block3);
//     myFree(block2);
//     myFree(block1);


//     // Clean up
//     cleanupMemory(); 
//     return 0;
// }

#include "my_malloc.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    initializeMemory();

    printf("Allocating and deallocating in order...\n");
    void* block1 = myMalloc(100);
    if (block1) printf("Allocated 100 bytes for block1\n");
    void* block2 = myMalloc(50);
    if (block2) printf("Allocated 50 bytes for block2\n");
    
    myFree(block2);
    printf("Freed block2\n");
    myFree(block1);
    printf("Freed block1\n");

    printf("\nAllocating and deallocating out of order...\n");
    block1 = myMalloc(30);
    if (block1) printf("Allocated 30 bytes for block1\n");
    block2 = myMalloc(40);
    if (block2) printf("Allocated 40 bytes for block2\n");
    void* block3 = myMalloc(20);
    if (block3) printf("Allocated 20 bytes for block3\n");

    myFree(block2);
    printf("Freed block2 (out of order)\n");
    myFree(block1);
    printf("Freed block1\n");
    myFree(block3);
    printf("Freed block3\n");

    printf("\nTesting allocation when the stack is full...\n");
    void* block4 = myMalloc(MEMORY_SIZE-94); // Attempt to allocate the entire pool
    if (block4) {
        // printf("Allocated MEMORY_SIZE bytes for block4\n");
        // myFree(block4);
        // printf("Freed block4\n");
    } else {
        printf("Failed to allocate MEMORY_SIZE bytes for block4\n");
    }
    void* block5 = myMalloc(100);
 if (block5) {
        printf("Allocated MEMORY_SIZE bytes for block5\n");
        myFree(block5);
        printf("Freed block5\n");
    } else {
        printf("Failed to allocate MEMORY_SIZE bytes for block5\n");
    }
    return 0;
}
