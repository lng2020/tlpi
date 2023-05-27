#include "tlpi_hdr.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>

#define BUF_SIZE 1024

int main(int argc, char *argv[]) {
  if (argc != 2) {
    usageErr("%s filepath\n", argv[0]);
  }
  DIR *dirp = opendir("/proc");
  if (dirp == NULL) {
    errExit("opendir");
  }
  struct dirent *dp;
  while ((dp = readdir(dirp)) != NULL) {
    if (dp->d_type != DT_DIR) {
      continue;
    }
    char path[BUF_SIZE];
    snprintf(path, BUF_SIZE, "/proc/%s/fd", dp->d_name);
    DIR *fdDirp = opendir(path);
    if (fdDirp == NULL) {
      continue;
    }
    struct dirent *fdDp;
    while ((fdDp = readdir(fdDirp)) != NULL) {
      if (fdDp->d_type != DT_LNK) {
        continue;
      }
      char fdPath[BUF_SIZE];
      snprintf(fdPath, BUF_SIZE, "/proc/%s/fd/%s", dp->d_name, fdDp->d_name);
      char link[BUF_SIZE];
      ssize_t numRead = readlink(fdPath, link, BUF_SIZE);
      if (numRead == -1) {
        continue;
      }
      link[numRead] = '\0';
      if (strcmp(link, argv[1]) == 0) {
        printf("%s\n", dp->d_name);
      }
    }
  }
}