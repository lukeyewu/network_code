#include <stdio.h>
#include <memory.h>
#include <unistd.h>
#include <signal.h>

void timeout(int sig)
{
    if(sig == SIGALRM)
        puts("time out!");

    alarm(2);
}

int main(int argc, char ** argv)
{
    int ii;
    struct sigaction act;
    memset(&act, 0, sizeof(struct sigaction));
    act.sa_handler = timeout;

    sigaction(SIGALRM, &act, 0);

    alarm(2);
    for(ii=0; ii<3; ++ii)
    {
        puts("wait ......");
        sleep(100);
    }

    return 0;
}
