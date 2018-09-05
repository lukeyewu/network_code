#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <memory.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30

void error_handling()
{
    fputs("message", stderr);
    fputs("\n", stderr);
    exit(1);
}

void read_childproc(int sig)
{
    int status;
    pid_t pid = waitpid(-1, &status, WNOHANG);
    printf("removed child proc %d\n", pid);
}

int main(int argc, char **argv)
{
    struct sigaction act;
    memset(&act, 0, sizeof(struct sigaction));
    act.sa_handler = read_childproc;
    int state = sigaction(SIGCHLD, &act, 0);

    int serv_sock, clnt_sock;
    struct sockaddr_in serv_addr, clnt_addr;
    pid_t pid;
    socklen_t addr_size;
    int str_len;
    char buf[BUF_SIZE];
    
    if(argc != 2)
    {
        printf("USage: %s<port>\n", argv[0]);
        exit(0);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("bind() error");

    if(listen(serv_sock, 5) == -1)
        error_handling("listen() error");

    while(1)
    {
        addr_size = sizeof(clnt_addr);
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &addr_size);
        if(clnt_sock == -1)
            continue;
        else
        {
            puts("New client connected...");
        }

        pid = fork();
        if(pid == -1)
        {
            close(clnt_sock);
            continue;
        }

        if(pid != 0)
        {
            close(clnt_sock);
        }
        else  // child process, process function
        {
            close(serv_sock);

            while((str_len=read(clnt_sock, buf, BUF_SIZE)) !=0 )
            {
                write(clnt_sock, buf, str_len);
            }

            close(clnt_sock);
            puts("client disconnected...");
            return 0;
        }
    }

    close(serv_sock);
    return 0;
}

