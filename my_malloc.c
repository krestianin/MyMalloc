#include "my_malloc.h"
#include <stdlib.h>
#include <stdio.h>
#include <climits>

static char* memory_pool = NULL;
static char* stack_top = NULL;
MemoryBlock* root = NULL;

//Initialization of memory pool
void initializeMemory() {
    memory_pool = (char*)malloc(MEMORY_SIZE);
    if (memory_pool == NULL) {
        fprintf(stderr, "Error allocating memory pool\n");
        exit(EXIT_FAILURE);
    }
    stack_top = memory_pool;
}



//Utility functions of AVL-Tree

int height(MemoryBlock* N) {
    if (N == NULL)
        return 0;
    return N->height;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

MemoryBlock* newMemoryBlock(void* ptr, unsigned short size) {
    MemoryBlock* block = (MemoryBlock*)malloc(sizeof(MemoryBlock));
    block->ptr = ptr;
    block->size = size;
    block->left = NULL;
    block->right = NULL;
    block->height = 1;
    return block;
}

MemoryBlock* rightRotate(MemoryBlock* y) {
    MemoryBlock* x = y->left;
    MemoryBlock* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

MemoryBlock* leftRotate(MemoryBlock* x) {
    MemoryBlock* y = x->right;
    MemoryBlock* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

int getBalance(MemoryBlock* N) {
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
}

MemoryBlock* insertMemoryBlock(MemoryBlock* node, void* ptr, unsigned short size) {

    if (node == NULL)
        return newMemoryBlock(ptr, size);

    if (size < node->size)
        node->left = insertMemoryBlock(node->left, ptr, size);
    else if (size > node->size)
        node->right = insertMemoryBlock(node->right, ptr, size);
    else
        return node;

    node->height = 1 + max(height(node->left), height(node->right));

    int balance = getBalance(node);

    // If this node becomes unbalanced, then there are 4 cases

    // Left Left Case
    if (balance > 1 && size < node->left->size)
        return rightRotate(node);

    // Right Right Case
    if (balance < -1 && size > node->right->size)
        return leftRotate(node);

    // Left Right Case
    if (balance > 1 && size > node->left->size) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Right Left Case
    if (balance < -1 && size < node->right->size) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
    return node;
}

MemoryBlock* findClosestBlock(MemoryBlock* node, unsigned short size) {
    MemoryBlock* current = node;
    MemoryBlock* best_fit = NULL;

    while (current != NULL) {
        if (current->size >= size) {
            if (best_fit == NULL || current->size < best_fit->size) {
                best_fit = current;
            }
            current = current->left;
        } else {
            current = current->right;
        }
    }

    return best_fit;
}

MemoryBlock* minValueNode(MemoryBlock* node) {
    MemoryBlock* current = node;

    // Loop down to find the leftmost leaf
    while (current->left != NULL)
        current = current->left;

    return current;
}

MemoryBlock* deleteNode(MemoryBlock* root, unsigned short size) {

    if (root == NULL)
        return root;

    if (size < root->size)
        root->left = deleteNode(root->left, size);

    else if (size > root->size)
        root->right = deleteNode(root->right, size);

    else {

        if ((root->left == NULL) || (root->right == NULL)) {
            MemoryBlock* temp = root->left ? root->left : root->right;

            if (temp == NULL) {
                temp = root;
                root = NULL;
            } else
                *root = *temp;

            free(temp);
        } else {

            MemoryBlock* temp = minValueNode(root->right);

            root->size = temp->size;
            root->ptr = temp->ptr;

            root->right = deleteNode(root->right, temp->size);
        }
    }

    if (root == NULL)
        return root;

    root->height = 1 + max(height(root->left), height(root->right));

    int balance = getBalance(root);


    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);

    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);

    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}


//my_malloc implementation
void* my_malloc(size_t size) {
    unsigned short total_size = size + sizeof(unsigned short);
   if (stack_top + total_size > memory_pool + MEMORY_SIZE) {
     printf("allocating from AVL\n");

        MemoryBlock* block = findClosestBlock(root, size);
        if (block) {
            root = deleteNode(root, block->size);
            return block->ptr;
        }
        return NULL;
    }
       printf("allocating from pool\n");

    if (size > USHRT_MAX - sizeof(unsigned short)) {
        printf("Block size is too large to be tracked with an unsigned short");
        return NULL;
    }

    *((unsigned short*)stack_top) = (unsigned short)size;
    void* allocated_memory = stack_top + sizeof(unsigned short);

    stack_top += total_size;

    return allocated_memory;
}


//my_free implementation
void my_free(void* ptr) {
    if (!ptr) {
        return;
    }

    unsigned short block_size = *((unsigned short*)((char*)ptr - sizeof(unsigned short)));
    char* expected_block_start = (char*)ptr - sizeof(unsigned short);

    if (expected_block_start == stack_top - block_size - sizeof(unsigned short)) {
        stack_top = expected_block_start;
    } else {
        root = insertMemoryBlock(root, ptr, block_size);
    }

    stack_top = expected_block_start;
}

//clean memory pool
void cleanupMemory() {
    free(memory_pool);
}