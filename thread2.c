#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

void * thread_main(void* arg)
{
    int ii;

    char *msg = (char *)malloc(30*sizeof(char));
    strcpy(msg, "Hello,  I am a thread\n");
    int cnt = *((int *)arg);
    for(ii=0; ii<cnt; ++ii)
    {
        sleep(1);
        puts("running thread");
    }

    return (void *)msg;
}


int main(int argc, char **argv)
{
    pthread_t t_id;
    int thread_param = 5;
    void *thr_ret;

    if(pthread_create(&t_id, NULL, thread_main, (void *)&thread_param) != 0)
    {
        puts("pthread_create() error");
        return -1;
    }

    if(pthread_join(t_id, &thr_ret) != 0)
    {
        puts("pthread_join() error");
        return -1;
    }

    printf ("Thread return message :  %s\n",  (char *)thr_ret);

    return 0;
}
