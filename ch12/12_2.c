#include "tlpi_hdr.h"
#include <dirent.h>
#include <fcntl.h>
#include <stdlib.h>

#define BUF_SIZE 1024

void my_pstree(int indent, pid_t pid) {
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
    snprintf(path, BUF_SIZE, "/proc/%s/status", dp->d_name);
    int fd = open(path, O_RDONLY);
    if (fd == -1) {
      continue;
    }
    char buf[BUF_SIZE];
    ssize_t numRead = read(fd, buf, BUF_SIZE);
    if (numRead == -1) {
      continue;
    }
    // read `PPid: \t <ppid>\n`
    pid_t ppid = -1;
    char name[BUF_SIZE + 1];
    char *saveptr;
    char *token = strtok_r(buf, "\n", &saveptr);
    while (token != NULL) {
      if (strncmp(token, "Name:\t", 6) == 0) {
        strncpy(name, token + 6, BUF_SIZE);
      } else if (strncmp(token, "PPid:\t", 6) == 0) {
        ppid = strtol(token + 6, NULL, 10);
        break;
      }
      token = strtok_r(NULL, "\n", &saveptr);
    }
    // print pid if ppid matches
    if (ppid == pid) {
      for (int i = 0; i < indent; ++i) {
        printf("  ");
      }
      printf("%s(%s)\n", dp->d_name, name);
      my_pstree(indent + 1, strtol(dp->d_name, NULL, 10));
    }
  }
}

int main() {
  my_pstree(0, 1);
  return 0;
}