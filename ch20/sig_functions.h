#ifndef SIG_FUNCTIONS_H
#define SIG_FUNCTIONS_H
#include <signal.h>
#include <stdio.h>
#include <string.h>

void printSigset(FILE *of, const char *prefix, const sigset_t *sigset);
int printSigMask(FILE *of, const char *msg);
int printPendingSigs(FILE *of, const char *msg);

#endif