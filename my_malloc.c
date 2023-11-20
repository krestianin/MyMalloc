// #include <stddef.h>
// #include "my_malloc.h"
// #include "stdio.h"



// char memory_pool[MEMORY_SIZE];
// struct MemoryBlock* free_list = NULL;

// void initializeMemory() {
//     // Initialize a single memory block that covers the entire pool
//     struct MemoryBlock* initial_block = (struct MemoryBlock*)memory_pool;
//     initial_block->size = MEMORY_SIZE;
//     initial_block->status = 0; // Free
//     initial_block->next = NULL;
//     initial_block->memory = memory_pool + sizeof(struct MemoryBlock);
    
//     // Set the initial block as the head of the free list
//     free_list = initial_block;
// }

// void* myMalloc(size_t size) {
//     struct MemoryBlock* current_block = free_list;
//     struct MemoryBlock* prev_block = NULL;

//     // Find a suitable free block from the free list
//     while (current_block && (current_block->status == 1 || current_block->size < size)) {
//         prev_block = current_block;
//         current_block = current_block->next;
//     }

//     if (!current_block) {
//         // No suitable block found
//         return NULL;
//     }

//     // If the block is larger than needed, split it
//     if (current_block->size > size + sizeof(struct MemoryBlock)) {
//         struct MemoryBlock* next_block = (struct MemoryBlock*)((char*)current_block + size + sizeof(struct MemoryBlock));
//         next_block->size = current_block->size - size - sizeof(struct MemoryBlock);
//         next_block->status = 0; // Free
//         next_block->next = current_block->next;
//         next_block->memory = (char*)next_block + sizeof(struct MemoryBlock);

//         current_block->size = size;
//         current_block->next = next_block;
//     }

//     // Update the block status and remove it from the free list
//     if (prev_block) {
//         prev_block->next = current_block->next;
//     } else {
//         free_list = current_block->next;
//     }
//     current_block->status = 1; // Allocated

//     return current_block->memory;
// }

// void myFree(void* ptr) {
//     if (!ptr) {
//         // Nothing to free
//         return;
//     }

//     // Mark the corresponding memory block as free
//     struct MemoryBlock* block_to_free = (struct MemoryBlock*)((char*)ptr - sizeof(struct MemoryBlock));
//     block_to_free->status = 0;

//     // Push the block onto the free list
//     block_to_free->next = free_list;
//     free_list = block_to_free;
// }
#include "my_malloc.h"
#include <stdlib.h>
#include <stdio.h>
// #include <climits>

static char* memory_pool = NULL;
static char* stack_top = NULL;

void initializeMemory() {
    memory_pool = (char*)malloc(MEMORY_SIZE);
    if (memory_pool == NULL) {
        fprintf(stderr, "Error allocating memory pool\n");
        exit(EXIT_FAILURE);
    }
    stack_top = memory_pool;
}
// Utility function to get the height of the tree
int height(MemoryBlock* N) {
    if (N == NULL)
        return 0;
    return N->height;
}

// Utility function to get the maximum of two integers
int max(int a, int b) {
    return (a > b) ? a : b;
}

// A utility function to create a new memory block node
MemoryBlock* newMemoryBlock(void* ptr, size_t size) {
    MemoryBlock* block = (MemoryBlock*)malloc(sizeof(MemoryBlock));
    block->ptr = ptr;
    block->size = size;
    block->left = NULL;
    block->right = NULL;
    block->height = 1; // new node is initially added at leaf
    return block;
}

// A utility function to right rotate subtree rooted with y
MemoryBlock* rightRotate(MemoryBlock* y) {
    MemoryBlock* x = y->left;
    MemoryBlock* T2 = x->right;

    // Perform rotation
    x->right = y;
    y->left = T2;

    // Update heights
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    // Return new root
    return x;
}

// A utility function to left rotate subtree rooted with x
MemoryBlock* leftRotate(MemoryBlock* x) {
    MemoryBlock* y = x->right;
    MemoryBlock* T2 = y->left;

    // Perform rotation
    y->left = x;
    x->right = T2;

    // Update heights
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    // Return new root
    return y;
}

// Get balance factor of node N
int getBalance(MemoryBlock* N) {
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
}

MemoryBlock* insertMemoryBlock(MemoryBlock* node, void* ptr, size_t size) {
    /* 1. Perform the normal BST insertion */
    if (node == NULL)
        return newMemoryBlock(ptr, size);

    if (size < node->size)
        node->left = insertMemoryBlock(node->left, ptr, size);
    else if (size > node->size)
        node->right = insertMemoryBlock(node->right, ptr, size);
    else // Equal sizes are not allowed in AVL tree
        return node;

    /* 2. Update height of this ancestor node */
    node->height = 1 + max(height(node->left), height(node->right));

    /* 3. Get the balance factor of this ancestor node to check whether this node became unbalanced */
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

    /* return the (unchanged) node pointer */
    return node;
}

MemoryBlock* findClosestBlock(MemoryBlock* node, size_t size) {
    MemoryBlock* current = node;
    MemoryBlock* best_fit = NULL;

    while (current != NULL) {
        if (current->size >= size) {
            // This node is a potential fit
            if (best_fit == NULL || current->size < best_fit->size) {
                best_fit = current;
            }
            // Move to the left child to find a smaller or equal block
            current = current->left;
        } else {
            // Move to the right child to find a larger block
            current = current->right;
        }
    }

    return best_fit;
}
// Utility function to find the node with minimum size value in the tree
MemoryBlock* minValueNode(MemoryBlock* node) {
    MemoryBlock* current = node;

    // Loop down to find the leftmost leaf
    while (current->left != NULL)
        current = current->left;

    return current;
}

// Recursive function to delete a node with given size from subtree with given root.
// It returns root of the modified subtree.
MemoryBlock* deleteNode(MemoryBlock* root, size_t size) {
    // STEP 1: Perform standard BST delete
    if (root == NULL)
        return root;

    // If the size to be deleted is smaller than the root's size,
    // then it lies in left subtree
    if (size < root->size)
        root->left = deleteNode(root->left, size);

    // If the size to be deleted is greater than the root's size,
    // then it lies in right subtree
    else if (size > root->size)
        root->right = deleteNode(root->right, size);

    // If size is same as root's size, then this is the node to be deleted
    else {
        // Node with only one child or no child
        if ((root->left == NULL) || (root->right == NULL)) {
            MemoryBlock* temp = root->left ? root->left : root->right;

            // No child case
            if (temp == NULL) {
                temp = root;
                root = NULL;
            } else // One child case
                *root = *temp; // Copy the contents of the non-empty child

            free(temp);
        } else {
            // Node with two children: Get the inorder successor (smallest
            // in the right subtree)
            MemoryBlock* temp = minValueNode(root->right);

            // Copy the inorder successor's data to this node
            root->size = temp->size;
            root->ptr = temp->ptr;

            // Delete the inorder successor
            root->right = deleteNode(root->right, temp->size);
        }
    }

    // If the tree had only one node then return
    if (root == NULL)
        return root;

    // STEP 2: Update the height of the current node
    root->height = 1 + max(height(root->left), height(root->right));

    // STEP 3: Get the balance factor of this node (to check whether
    // this node became unbalanced)
    int balance = getBalance(root);

    // If this node becomes unbalanced, then there are 4 cases

    // Left Left Case
    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);

    // Left Right Case
    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    // Right Right Case
    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);

    // Right Left Case
    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}


void* myMalloc(size_t size) {
    size_t total_size = size + sizeof(size_t);
   if (stack_top + total_size > memory_pool + MEMORY_SIZE) {
        // Search the AVL tree for a suitable block
        MemoryBlock* block = findClosestBlock(root, size);
        if (block) {
            // Remove the block from the AVL tree
            root = deleteNode(root, block->size);
            return block->ptr;
        }
        return NULL;
    }

    if (size > 65535 - sizeof(unsigned short)) {
        // Block size is too large to be tracked with an unsigned short
        return NULL;
    }

    // unsigned short total_size = (unsigned short)size + sizeof(unsigned short);
    // if (stack_top + total_size > memory_pool + MEMORY_SIZE) {
    //     return NULL;
    // }

    *((unsigned short*)stack_top) = (unsigned short)size;
    void* allocated_memory = stack_top + sizeof(unsigned short);

    stack_top += total_size;

    return allocated_memory;
}

void myFree(void* ptr) {
    if (!ptr) {
        return;
    }

    unsigned short block_size = *((unsigned short*)((char*)ptr - sizeof(unsigned short)));
    char* expected_block_start = (char*)ptr - sizeof(unsigned short);

    if (expected_block_start == stack_top - block_size - sizeof(unsigned short)) {
        stack_top = expected_block_start;
    } else {
        // Block is not at the top, add to AVL tree
        root = insertMemoryBlock(root, ptr, block_size);
    }

    stack_top = expected_block_start;
}


void cleanupMemory() {
    free(memory_pool);  // Free the allocated memory pool
}