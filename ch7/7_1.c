#include "tlpi_hdr.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_ALLOCS 1000000

int main(int argc, char* argv[]){
    char* ptr[MAX_ALLOCS];
    int blockSize, numAllocs, j;

    printf("\n");

    if (argc < 3 || strcmp(argv[1], "--help") == 0){
        usageErr("%s num-allocs block-size\n", argv[0]);
    }

    numAllocs = getInt(argv[1], GN_GT_0, "num-allocs");
    if (numAllocs > MAX_ALLOCS){
        cmdLineErr("num-allocs > %d\n", MAX_ALLOCS);
    }

    blockSize = getInt(argv[2], GN_GT_0 | GN_ANY_BASE, "block-size");

    printf("Initial program break: %10p\n", sbrk(0));
    for (j = 0; j < numAllocs; j++){
        ptr[j] = malloc(blockSize);
        if (ptr[j] == NULL){
            errExit("malloc");
        }
        printf("Program break is now: %10p\n", sbrk(0));
    }
    exit(EXIT_SUCCESS);
}