#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREAD 100

volatile long long sum = 0;
void * thread_inc(void *arg)
{
    int ii;
    for(ii=0; ii<5000000; ++ii)
    {
        sum += 1;
    }

    return NULL;
}

void *thread_dec(void *arg)
{
    int ii;
    for(ii=0; ii<5000000; ++ii)
    {
        sum -= 1;
    }

    return NULL;
}

int main(int argc, char **argv)
{
    pthread_t thread_id[NUM_THREAD];
    int ii;

    for(ii=0; ii<NUM_THREAD; ++ii)
    {
        if(ii&0x01 == 0)
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

    return 0;
}
