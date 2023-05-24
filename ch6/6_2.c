#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>

static jmp_buf env;

void doJump(){
    printf("Jumping\n");
    longjmp(env, 1);
}

void doSet(){
    int i = 7;
    printf("Setting\n");
    if (setjmp(env) == 1) {
        printf("after jump:i = %d\n", i);
    } else {
        printf("before jump:i = %d\n", i);
    }
}

int main(int argc, char* argv[]){
    doSet();
    doJump();
}
