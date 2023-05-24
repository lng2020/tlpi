#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <errno.h>
#include <string.h>

int my_setenv(const char *name, const char* value, int overwrite){
    if (name == NULL || name[0] == '\0' || strchr(name, '=') != NULL || value == NULL) {
        errno = EINVAL;
        return -1;
    }
    int len = strlen(name) + strlen(value) + 2;
    char *envbuf = malloc(len);
    if (envbuf == NULL) {
        return -1;
    }
    snprintf(envbuf, len, "%s=%s", name, value);
    return putenv(envbuf);
}

int my_unsetenv(const char *name){
    if (name == NULL || name[0] == '\0' || strchr(name, '=') != NULL) {
        errno = EINVAL;
        return -1;
    }
    int len = strlen(name);
    char *envbuf = malloc(len);
    if (envbuf == NULL) {
        return -1;
    }
    snprintf(envbuf, len, "%s", name);
    return putenv(envbuf);
}