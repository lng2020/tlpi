#include "tlpi_hdr.h"
#include "fcntl.h"

int main(){
    int fd1, fd2, fd3;
    fd1 = open("test.txt", O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    fd2 = dup(fd1);
    fd3 = dup2(fd1, 100);
    write(fd1, "Hello,", 6);
    write(fd2, "world", 6);
    lseek(fd2, 0, SEEK_SET);
    write(fd1, "HELLO,", 6);
    write(fd3, "Gidday", 6);
    return 0;
}