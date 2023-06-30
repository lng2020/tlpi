#include <signal.h>
#include <tlpi_hdr.h>
#include <unistd.h>

static void sigHandler(int sig)
{
    printf("received signal %d (%s)\n", sig, strsignal(sig));
    sleep(5);
    printf("done handling signal %d (%s)\n", sig, strsignal(sig));
}

int main(int argc, char *argv[])
{
    sigaction(SIGINT, &(struct sigaction){.sa_handler = sigHandler}, NULL);
    while (1);
    return 0;
}