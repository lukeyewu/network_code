#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void * thread_main(void* arg)
{
    int ii;
    int cnt = *((int *)arg);
    for(ii=0; ii<cnt; ++ii)
    {
        sleep(1);
        puts("running thread");
    }

    return NULL;
}


int main(int argc, char **argv)
{
    pthread_t t_id;
    int thread_param = 5;
    
    if(pthread_create(&t_id, NULL, thread_main, (void *)&thread_param) != 0)
    {
        puts("pthread_create() error");
        return -1;
    }

    sleep(10);
    puts("end of main()");

    return 0;
}
