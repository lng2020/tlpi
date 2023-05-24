#include <fcntl.h>
#include "tlpi_hdr.h"
#include <sys/types.h>
#include <sys/uio.h>

ssize_t my_readv(int fd, const struct iovec *iov, int iovcnt) {
    ssize_t total = 0;
    for (int i = 0; i < iovcnt; i++) {
        total += iov[i].iov_len;
    }

    char *buf = malloc(total);
    if (buf == NULL) {
        return -1;
    }

    ssize_t nread = read(fd, buf, total);
    if (nread == -1) {
        free(buf);
        return -1;
    }

    char *p = buf;
    for (int i = 0; i < iovcnt; i++) {
        memcpy(iov[i].iov_base, p, iov[i].iov_len);
        p += iov[i].iov_len;
    }

    free(buf);
    return nread;
}

ssize_t my_writev(int fd, const struct iovec *iov, int iovcnt) {
    ssize_t total = 0;
    for (int i = 0; i < iovcnt; i++) {
        total += iov[i].iov_len;
    }

    char *buf = malloc(total);
    if (buf == NULL) {
        return -1;
    }

    char *p = buf;
    for (int i = 0; i < iovcnt; i++) {
        memcpy(p, iov[i].iov_base, iov[i].iov_len);
        p += iov[i].iov_len;
    }

    ssize_t nwrite = write(fd, buf, total);
    free(buf);

    return nwrite;
}