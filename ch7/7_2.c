#include <unistd.h>
#include <string.h>
#include <assert.h>

typedef struct Block {
    size_t size;
    struct Block* next;
    int free;
    int padding;  // for alignment
    char data[1];  // use this array as pointer to the block
} Block;

#define BLOCK_SIZE sizeof(Block)

Block* head = NULL;

Block* find_free_block(size_t size) {
    Block* curr = head;
    while (curr && !(curr->free && curr->size >= size)) {
        curr = curr->next;
    }
    return curr;
}

Block* extend_heap(Block* last, size_t size) {
    Block* block;
    block = sbrk(0);
    void* request = sbrk(size + BLOCK_SIZE);
    assert ((void*)block == request);  // not thread safe
    if (request == (void*) -1) {
        return NULL;  // sbrk failed
    }

    if (last) {  // NULL on first request
        last->next = block;
    }
    block->size = size;
    block->next = NULL;
    block->free = 0;
    return block;
}

void* malloc(size_t size) {
    Block* block;
    if (size <= 0) {
        return NULL;
    }

    if (!head) {  // first call
        block = extend_heap(NULL, size);
        if (!block) {
            return NULL;
        }
        head = block;
    } else {
        Block* last = head;
        block = find_free_block(size);
        if (!block) {  // failed to find free block
            block = extend_heap(last, size);
            if (!block) {
                return NULL;
            }
        } else {  // found free block
            // TODO: consider splitting block here
            block->free = 0;
        }
    }

    return block->data;
}

void free(void* ptr) {
    if (!ptr) {
        return;
    }

    // TODO: consider merging blocks once splitting blocks is implemented
    Block* block = (Block*)ptr - 1;
    assert(block->free == 0);
    block->free = 1;
}
