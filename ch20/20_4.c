#include <signal.h>
#include <tlpi_hdr.h>

static void my_siginterrupt(int sig, int flag)
{
    struct sigaction sa;
    if (sigaction(sig, NULL, &sa) == -1)
        errExit("sigaction");
    if (flag)
        sa.sa_flags &= ~SA_RESTART;
    else
        sa.sa_flags |= SA_RESTART;
    if (sigaction(sig, &sa, NULL) == -1)
        errExit("sigaction");
}