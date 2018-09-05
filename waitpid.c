#include <stdio.h>

#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char **argv)
{
    int status;
    pid_t pid = fork();

    if(pid == 0)
    {
        sleep(18);
        return 15;
    }
    else
    {
        while(!waitpid(-1, &status, WNOHANG))
        {
            sleep(3);
            puts("sleep 3 secs.");
        }

        if(WIFEXITED(status))
        {
            printf("pid = %d\n", WEXITSTATUS(status));
        }   
    }
    

    return 0;
}
