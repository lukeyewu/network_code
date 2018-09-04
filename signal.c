#include <stdio.h>

#include <unistd.h>
#include <signal.h>

void timeout(int sig)
{
    if(sig == SIGALRM)
        puts("Time out!");
    alarm(2);
}

void keycontroller(int sig)
{
    if (sig == SIGINT)
        puts("CTRL + C pressed.");
}

int main(int argc, char **argv)
{
    int ii;
    signal(SIGALRM, timeout);
    signal(SIGINT, keycontroller);
    alarm(2);

    for(ii=0; ii<3; ++ii)
    {
        puts("wait ......");
        sleep(100);
    }

    return 0;
}
