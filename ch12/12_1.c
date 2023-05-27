#include "fcntl.h"
#include "tlpi_hdr.h"
#include <dirent.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define BUF_SIZE 1024

uid_t userIdFromName(const char *name) {
  struct passwd *pwd;
  uid_t u;
  char *endptr;
  if (name == NULL || *name == '\0') {
    return -1;
  }
  u = strtol(name, &endptr, 10);
  if (*endptr == '\0') {
    return u;
  }
  pwd = getpwnam(name);
  if (pwd == NULL) {
    return -1;
  }
  return pwd->pw_uid;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    usageErr("%s username\n", argv[0]);
  }
  uid_t uid = userIdFromName(argv[1]);
  if (uid == -1) {
    fatal("Couldn't find uid for %s\n", argv[1]);
  }
  DIR *dirp = opendir("/proc");
  if (dirp == NULL) {
    errExit("opendir");
  }
  struct dirent *dp;
  while ((dp = readdir(dirp)) != NULL) {
    // open /proc/PID/status, if uid matches, print the pid and name
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
    // read `Name: \t <name>\n` and `Uid: \t <uid>\n`
    char *name = NULL;
    char *uidStr = NULL;
    uid_t uid_;
    char *saveptr;
    char *token = strtok_r(buf, "\n", &saveptr);
    while (token != NULL) {
      if (strncmp(token, "Name:", 5) == 0) {
        name = token + 6;
      } else if (strncmp(token, "Uid:", 4) == 0) {
        uidStr = token + 5;
        sscanf(uidStr, "%d", &uid_);
      }
      token = strtok_r(NULL, "\n", &saveptr);
    }
    if (name == NULL || uidStr == NULL) {
      continue;
    }
    if (uid_ == uid) {
      printf("%s %s\n", dp->d_name, name);
    }
  }
}