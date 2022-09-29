#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct Block {
    int block_size;
    struct Block* next_block;
};

const OVERHEAD_SIZE = sizeof(struct Block); // refers to size & pointer of a block // TODO
const POINTER_SIZE = sizeof(void*);
struct Block* free_head; // * points to the first free block in the first list

void my_initialize_heap(int size) { // ! DONE
    //YOUR CODE
    free_head  = (struct Block*) malloc(size);
    free_head -> block_size = size - OVERHEAD_SIZE;
    free_head -> next_block = NULL;

} // * my_initialize_heap(int size)

void* my_alloc(int size) { // * Fills an allocation request
    if (size <= 0) {
        printf("Size must be greater than 0");
        return 0;
    }

    if (size % POINTER_SIZE != 0){
        printf("Size must be a multiple of the pointer size");
        return 0;
    }

    struct Block* curr = free_head; // * pointer starts at free head
    struct Block* prev = 0;
    bool found = false;

    while (curr != NULL) {
        if (curr->block_size >= size){ // * A block is found (First Fit Heuristic)
            found = true;

            // ? Verify if this is right
            if (curr->block_size  > size + OVERHEAD_SIZE){  // * Splittable


                // ! The block that is allocated  is the free head
                //  ! the new_block is the block that has the block_size of (block_size - overhead)

                // * ---- Allocates new block ----
                struct Block* new_block = curr; // * byte location is located at the current block

                // * Updating curr block block
                curr-> block_size = (new_block->block_size) - size + OVERHEAD_SIZE; // * Original block size minus size and overhead
                curr->next_block = NULL;

                new_block->block_size = size + OVERHEAD_SIZE;
                new_block->next_block = new_block + 1;

                return new_block; // * returns allocated block
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

    if (!found) return 0; // * No block was found
} // * my_alloc(int size)

void my_free(void *data){ // * deallocates a value
    struct Block* freed_block = (struct Block*) (( char *) data - OVERHEAD_SIZE) ;
    freed_block->next_block = NULL;
    free_head = NULL;

}
