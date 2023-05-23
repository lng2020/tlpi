#include "tlpi_hdr.h"
#include <fcntl.h>
#include <unistd.h>

#ifndef BUF_SIZE
#define BUF_SIZE 1024
#endif

int main(int argc, char *argv[]) {
  if (argc < 2 || argc > 3 || strcmp(argv[1], "--help") == 0) {
    usageErr("%s [-a] file\n", argv[0]);
  }
  int opt;
  Boolean append = FALSE;
  while ((opt = getopt(argc, argv, "a")) != -1) {
    switch (opt) {
    case 'a':
      append = TRUE;
      break;
    default:
      break;
    }
  }
  char buffer[BUF_SIZE];
  char *filename = argv[optind];
  if (filename == NULL) {
    usageErr("%s [-a] file\n", argv[0]);
  }
  int fd;
  if (append) {
    fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
  } else {
    fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
  }
  ssize_t numRead;
  while ((numRead = read(STDIN_FILENO, buffer, 1024)) > 0) {
    if (write(STDOUT_FILENO, buffer, numRead) != numRead) {
      errExit("write stdout");
    }
    if (write(fd, buffer, numRead) != numRead) {
      errExit("write file");
    }
  }
  if (numRead == -1) {
    errExit("read");
  }
  if (close(fd) == -1) {
    errExit("close");
  }
  return 0;
}