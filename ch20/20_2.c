#include "sig_functions.h"
#include <signal.h>
#include <tlpi_hdr.h>

int main(int argc, char *argv[])
{
    for (int i = 1; i < NSIG; i++)
        signal(i,  SIG_IGN);
    while (1)
    {
        sigset_t pendingMask;
        if (sigpending(&pendingMask) == -1)
            errExit("sigpending");
        printSigset(stdout, "\t\t", &pendingMask);
        sleep(1);
    }
}