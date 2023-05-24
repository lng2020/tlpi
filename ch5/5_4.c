// implement dup() and dup2()
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

int my_dup(int oldfd) {
    return fcntl(oldfd, F_DUPFD, 0);
}

int my_dup2(int oldfd, int newfd) {
    if (fcntl(oldfd, F_GETFL) == -1) {
        errno = EBADF;
        return -1;
    }
    if (oldfd == newfd) {
        return newfd;
    }
    close(newfd);
    return fcntl(oldfd, F_DUPFD, newfd);
}