#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char ** argv)
{
    int status;
    pid_t pid = fork();

    if(pid == 0)
    {
        sleep(10);
        return 15;
    }
    else
    {
        printf ("Child PID: %d \n", pid);
        pid = fork();

        if(pid == 0)
        {
            sleep(10);
            exit(150);
        }
        else
        {
            printf("Child PID: %d\n", pid);
            wait(&status);
            if(WIFEXITED(status))
            {
                printf("Child send one: %d\n", WEXITSTATUS(status));
            }

            wait(&status);
            if(WIFEXITED(status))
            {
                printf("Child send two: %d\n", WEXITSTATUS(status));
            }


            sleep(20);
        }


    }

    return 0;
}
