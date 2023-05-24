#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include "tlpi_hdr.h"

int main(int argc, char* argv[])
{
    int fd;
    fd = open("test.txt", O_WRONLY | O_APPEND);
    if (fd == -1)
        errExit("open");
    off_t off = lseek(fd, 0, SEEK_SET);
    if (off == -1)
        errExit("lseek");
    char buf[] = "hello world";
    ssize_t numWrite = write(fd, buf, sizeof(buf));
    if (numWrite == -1)
        errExit("write");
    printf("write %ld bytes\n", (long)numWrite);
    return 0;
}