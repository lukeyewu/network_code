#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <memory.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#define BUF_SIZE 100
#define MAX_CLNT 256

int clnt_cnt = 0;
int clnt_socks[MAX_CLNT];
pthread_mutex_t mutex;

void error_handling()
{
    fputs("message", stderr);
    fputs("\n", stderr);
    exit(1);
}

void send_msg(int clnt_sock, char *buf, int str_len)
{
    pthread_mutex_lock(&mutex);
    write(clnt_sock, buf, str_len);
    pthread_mutex_unlock(&mutex);
}

void * thread_main_handle_init(void *arg)
{
    int clnt_sock =*((int *)arg);
    int str_len = 0, ii;
    char msg[BUF_SIZE];

    while( (str_len=read(clnt_sock, msg, BUF_SIZE)) != 0 )
    {
        send_msg(clnt_sock, msg, str_len);
    }
    
    pthread_mutex_lock(&mutex);
    for(ii=0; ii<clnt_cnt; ++ii)
    {
        if(clnt_sock == clnt_socks[ii])
        {
            while(ii++<clnt_cnt-1)
            {
                clnt_socks[ii] = clnt_socks[ii+1];
            }
            break;
        }
    }
    clnt_cnt--;
    pthread_mutex_unlock(&mutex);
    close(clnt_sock);
    return NULL;
}

int main(int argc, char **argv)
{
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_addr, clnt_addr;
    int addr_size = sizeof(clnt_addr);
    pthread_t tid;

    if(argc != 2)
    {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    pthread_mutex_init(&mutex, NULL);

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
    {
        error_handling("bind() error");
    }
    if(listen(serv_sock,5) == -1)
    {
        error_handling("listen() error");
    }

    while(1)
    {
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &addr_size);

        if(clnt_sock == -1)
        {
            puts("accept() error");
            break;
        }

        pthread_mutex_lock(&mutex);
        clnt_socks[clnt_cnt++] = clnt_sock;
        pthread_mutex_unlock(&mutex);

        pthread_create(&tid, NULL, thread_main_handle_init, (void *)&clnt_sock);
        pthread_detach(tid);

        printf("Connected client IP: %s\n", inet_ntoa(clnt_addr.sin_addr));
    }

    close(serv_sock);
    return 0;
}
