#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct Block {
    int block_size;
    struct Block* next_block;
};

const int OVERHEAD_SIZE = sizeof(struct Block); // refers to size & pointer of a block // TODO
const int POINTER_SIZE = sizeof(void*);
struct Block* free_head; // * points to the first free block in the first list

struct Block* testHead;

void my_initialize_heap(int size) { // ! DONE
    //YOUR CODE
    free_head  = (struct Block*) malloc(size);
    free_head -> block_size = size - OVERHEAD_SIZE;
    free_head -> next_block = NULL;

    // testHead = free_head;

} // * my_initialize_heap(int size)

void* my_alloc(int size) { // * Fills an allocation request
    if (size <= 0) {
        printf("Size must be greater than 0");
        return 0;
    }

    if (size % POINTER_SIZE != 0){ 
        // ! Have to make size to be a multiple of the pointer size

        int fragmentation = size % POINTER_SIZE;
        size += fragmentation;
        // printf("Fragmentation: %d ",fragmentation);
        // return 0;
    }

    struct Block* curr = free_head; // * pointer starts at free head
    struct Block* prev = 0;
    bool found = false;

    while (curr != NULL) {
        if (curr->block_size >= size){ // * A block is found (First Fit Heuristic)
            found = true;

            // ? Verify if this is right
            if (curr->block_size  >= size + OVERHEAD_SIZE + POINTER_SIZE){  // * Splittable


                // ! The block that is allocated  is the free head
                //  ! the new_block is the block that has the block_size of (block_size - overhead)

                // * ---- Allocates new block ----
                struct Block* new_block = (struct Block*) (( char *) curr + size + OVERHEAD_SIZE) ; // * byte location is located at the current block
                new_block -> next_block = NULL;
                new_block -> block_size = curr -> block_size - size - OVERHEAD_SIZE;

                // * Updating curr block block
                curr->block_size = size + OVERHEAD_SIZE; // * Original block size minus size and overhead
                curr->next_block = new_block;
                
                free_head = new_block;

                // if (curr == free_head){
                //     free_head = new_block;
                // }
                // else{
                //     new_block->next_block = curr ->next_block;
                //     prev->next_block = new_block;
                // }

                return curr; // * returns allocated block
            }
            else{ // * Not splittable
                if (curr == free_head){ // * if the curr is the head
                    free_head = curr->next_block;
                }
                else{ // * curr is not the head
                    prev->next_block = curr->next_block;
                }
            }

            break;
        }
        else{ // * Current block doesn't fit
            // * -------- traversing heap --------
            prev = curr;
            curr = curr -> next_block;
        }

    }

    return (void*)(char*) curr + OVERHEAD_SIZE;
    // if (!found) return 0; // * No block was found
} // * my_alloc(int size)

void my_free(void *data){ // * deallocates a value
    struct Block* freed_block = (struct Block*) (( char *) data - OVERHEAD_SIZE) ;
    // struct Block* freed_block = data ;

    freed_block->next_block = free_head;
    free_head = freed_block;

}



void menuOptionOne() {
    printf("Hello World\n");
    int *numOne = my_alloc(sizeof(int));
    printf("Address of int A: %p\n", numOne);
    my_free(numOne);
    int *numTwo = my_alloc(sizeof(int));
    printf("Address of int B: %p\n", numTwo);
    };

void menuOptionTwo() {
    int *numOne = my_alloc(sizeof(int));
    printf("Address of int A: %p\n", numOne);
    int *numTwo = my_alloc(sizeof(int));
    printf("Address of int B: %p\n", numTwo);
    printf("Verifying Results...\n");
    int overheadPlusLarger = OVERHEAD_SIZE + sizeof(void*);
    printf("Size of overhead + larger of (the size of an integer; the minimum block size): %d bytes\n", overheadPlusLarger);
    printf("Address B - Address A: %d bytes \n", (int)numTwo - 
    (int)numOne);
};

void menuOptionThree() {
    int *numOne = my_alloc(sizeof(int));
    printf("Address of int A: %p\n", numOne);
    int *numTwo = my_alloc(sizeof(int));
    printf("Address of int B: %p\n", numTwo);
    int *numThree = my_alloc(sizeof(int));
    printf("Address of int C: %p\n", numThree);
    my_free(numTwo);
    printf("After freeing int B...\n");
    double *arr = my_alloc(2 * sizeof(double));
    printf("Address of array of 2 double values: %p\n", arr);
    int *numFour = my_alloc(sizeof(int));
    printf("Address of int D (should be the int B): %p\n", numFour);
};

void menuOptionFour() {
    int check = 0;
    char *charOne = my_alloc(sizeof(char));
    printf("Address of char A: %p\n", charOne);
    int *numTwo = my_alloc(sizeof(int));
    printf("Address of int B: %p\n", numTwo);
    int overheadPlusLarger = OVERHEAD_SIZE + sizeof(void*);
    printf("Size of overhead + larger of (the size of an integer; the  minimum block size): %d\n", overheadPlusLarger);
};

void menuOptionFive() {
    int *arr = my_alloc(80 * sizeof(int));
    int *numOne = my_alloc(sizeof(int));
    printf("Address of array: %p\n", arr);
    printf("Address of int A: %p\n", numOne);
    printf("Address of int value: %p\n", arr + 80 + sizeof(int));
    printf("Value of int A: %d\n", *numOne);
    printf("Difference betwween array and int A: %d\n", (int)numOne - 
    (int)arr);
    my_free(arr);
    printf("After freeing array...\n");
    printf("Address of int value: %p\n", numOne);
    printf("Value of int A: %d\n", *numOne);
}


void traverseHeap(){
    struct Block* curr = testHead;
    while (curr){
        printf("Memory Address: %p, Block Size: %d",curr,curr->block_size);
        curr = curr->next_block;
    }
}
int main(){

    my_initialize_heap(1000);
    // traverseHeap();
    // menuOptionOne();
    // menuOptionTwo();
    // menuOptionThree();
    // menuOptionFour();
    menuOptionFive();
    
    return 0;
}
