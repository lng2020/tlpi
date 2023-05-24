#include "tlpi_hdr.h"
#include <error_functions.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(int argc, char* argv[]){
    if (argc == 4) {
        int fd = open(argv[1], O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
        if (fd == -1)
            errExit("open");
        off_t off = lseek(fd, 0, SEEK_END);
        if (off == -1)
            errExit("lseek");
        for (int i = 0; i < atoi(argv[2]); i++) {
            if (write(fd, "x", 1) == -1)
                errExit("write");
        }
    } else {
        int fd = open(argv[1], O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
        if (fd == -1)
            errExit("open");
        for (int i = 0; i < atoi(argv[2]); i++) {
            if (write(fd, "y", 1) == -1)
                errExit("write");
        }
    }
    return 0;
}