#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <memory.h>

static int count = 0;

void read_childpro(int sig)
{
    int status;
    int id = waitpid(-1, &status, WNOHANG);
    if(WIFEXITED(status))
    {
        printf("Removed proc id: %d \n", id);
        printf("Child send: %d\n", WEXITSTATUS(status));
    }

    count++;
    printf("count = %d\n", count);
}

int main()
{
    pid_t pid;
    struct sigaction act;
    memset(&act, 0, sizeof(struct sigaction));
    act.sa_handler = read_childpro;

    sigaction(SIGCHLD, &act, 0);
    pid = fork();

    if(pid == 0)
    {
        puts("I am child process");
        sleep(10);
        return 15;
    }
    else
    {
        printf("Child process id %d\n", pid);
        pid = fork();
        if(pid == 0)
        {
            puts("I am chlid process");
            sleep(10);
            return 150;
        }
        else
        {
            int ii;
            printf("Child process id %d\n", pid);
            for(ii=0; ii<5; ++ii)
            {
                puts("wait......");
                sleep(5);
            }
        }
    }

    return 0;
}
