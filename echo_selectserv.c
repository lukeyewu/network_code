#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>

#define BUF_SIZE 100

void error_handling(char * message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

int main(int argc, char **argv)
{
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_addr, clnt_addr;
    int addr_size = sizeof(clnt_addr);
    int str_len;
    char buf[BUF_SIZE];
   
    fd_set reads, copy_reads;
    struct timeval timeout;
    int fd_max, fd_num, ii;

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

    FD_ZERO(&reads);
    FD_SET(serv_sock, &reads);
    fd_max = serv_sock;

    while(1)
    {
        copy_reads = reads;
        timeout.tv_sec = 10;
        timeout.tv_usec = 0;

        if( (fd_num=select(fd_max+1, &copy_reads, 0, 0, &timeout)) == -1)
        {
            puts("select() error!");
            break;
        }
        else if(fd_num == 0)
        {
            puts("time out...");
            continue;
        }
        
        for(ii=0; ii<fd_max+1; ++ii)
        {
            if(FD_ISSET(ii, &copy_reads))
            {
                if(ii == serv_sock)
                {
                    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &addr_size);
                    FD_SET(clnt_sock, &reads);
                    if(fd_max<clnt_sock)
                    {
                        fd_max = clnt_sock;
                    }
                    printf("connected client: %d \n", clnt_sock);
                }
                else
                {
                    str_len = read(ii, buf, BUF_SIZE);
                    if(str_len == 0)
                    {
                        FD_CLR(ii, &reads);
                        close(ii);
                        printf("close client: %d\n", ii);
                    }
                    else
                    {
                        write(ii, buf, str_len);
                    }
                }
            }
        }
    }
    
    close(serv_sock);
    return 0;
}
