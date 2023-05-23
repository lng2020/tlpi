#include "tlpi_hdr.h"
#include <fcntl.h>
#include <sys/types.h>

#ifndef BUF_SIZE
#define BUF_SIZE 1024
#endif

Boolean is_zero(char* buf, ssize_t numRead)
{
    for (int i = 0; i < numRead; i++)
        if (buf[i] != 0)
            return FALSE;
    return TRUE;
}


int main(int argc, char* argv[])
{
    int inputFd, outputFd, openFlags;
    mode_t filePerms;
    ssize_t numRead;
    char buf[BUF_SIZE];

    if (argc != 3 || strcmp(argv[1], "--help") == 0)
        usageErr("%s old-file new-file\n", argv[0]);
    
    inputFd = open(argv[1], O_RDONLY);
    if (inputFd == -1)
        errExit("opening file %s", argv[1]);
    
    openFlags = O_CREAT | O_WRONLY | O_TRUNC;
    filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |
                S_IROTH | S_IWOTH; // rw-rw-rw-
    outputFd = open(argv[2], openFlags, filePerms);
    if (outputFd == -1)
        errExit("opening file %s", argv[2]);

    // if the input file has file hole, the output file will also have file hole
    while ((numRead = read(inputFd, buf, BUF_SIZE)) > 0)
    {
        if (is_zero(buf, numRead))
        {
            if (lseek(outputFd, numRead, SEEK_CUR) == -1)
                errExit("lseek");
            continue;
        }else {
            if (write(outputFd, buf, numRead) != numRead)
                fatal("couldn't write whole buffer");
        }
    }
    if (numRead == -1)
        errExit("read");

    if (close(inputFd) == -1 || close(outputFd) == -1)
        errExit("close input");
    return 0;
}