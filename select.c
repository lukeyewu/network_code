#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>

#define BUF_SIZE 30

int main(int argc, char** argv)
{
    fd_set reads, tmps;
    FD_ZERO(&reads);
    FD_SET(0, &reads);

    int result, str_len;
    char buf[BUF_SIZE];
    struct timeval timeout;

    while(1)
    {
        tmps = reads;
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;
        result = select(1, &tmps, 0, 0, &timeout);  //only look at standard input, therefore maxfd is equaled to 1;
        if(result == 0)
        {
            puts("time out...");
        }
        else if(result == -1)
        {
            puts("select() error!");
        }
        else
        {
            if(FD_ISSET(0, &tmps))
            {
                str_len = read(0, buf, BUF_SIZE);
                buf[BUF_SIZE] = 0;
                printf ("Message from console: %s\n", buf);
            }
        }
    }

    return 0;
}

