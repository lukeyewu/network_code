#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREAD 100
pthread_mutex_t mutex;

volatile long long sum = 0;
void * thread_inc(void *arg)
{
    int ii;

    pthread_mutex_lock(&mutex);
    for(ii=0; ii<5000000; ++ii)
    {
        sum += 1;
    }
    pthread_mutex_unlock(&mutex);

    return NULL;
}

void *thread_dec(void *arg)
{
    int ii;
    pthread_mutex_lock(&mutex);
    for(ii=0; ii<5000000; ++ii)
    {
        sum -= 1;
    }
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main(int argc, char **argv)
{
    pthread_t thread_id[NUM_THREAD];
    int ii;

    pthread_mutex_init(&mutex, NULL);

    for(ii=0; ii<NUM_THREAD; ++ii)
    {
        if(ii&0x01)
        {
            pthread_create(&thread_id[ii], NULL, thread_inc, NULL);
        }
        else
        {
            pthread_create(&thread_id[ii], NULL, thread_dec, NULL);
        }
    }

    for (ii=0; ii<NUM_THREAD; ++ii)
    {
        pthread_join(thread_id[ii], NULL);
    }

    printf("sum = %lld \n", sum);
    
    pthread_mutex_destroy(&mutex);

    return 0;
}
