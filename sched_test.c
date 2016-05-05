#define _GNU_SOURCE
#include <stdio.h>
#include <sched.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

#define threadnum 2

int SetAffinity(void)
{
    cpu_set_t mask;
    CPU_ZERO(&mask);
    CPU_SET(0, &mask);
    return sched_setaffinity(0, sizeof(mask), &mask);
}
void Busy(void)
{
    int i = 0;
    while(i < 10000000)
        i++;
}

void *RunSleep(void *num)
{
    int *thrnum = (int *)num;
    for(int i = 0 ; i < 3 ; i++)
    {
        printf("Thread %d is running.\n", *thrnum);
        Busy();
    }
}

int SetPolicy(void) // If this function fails, try sudo.
{
    struct sched_param param;
    param.sched_priority = sched_get_priority_max(SCHED_FIFO);
    return sched_setscheduler(0, SCHED_FIFO, &param);
}

int main(int argc, char **argv)
{
    if(SetAffinity() != 0)
        printf("set cpu affinity error\n");
    if(argc > 1)
        if(SetPolicy() != 0)
            printf("set policy error\n");
    printf("Sched policy = %d\n" , sched_getscheduler(0));
    
    pthread_t pthreads[threadnum];
    int thr_id[threadnum];
    for(int i = 0 ; i < threadnum ; i++)
    {
        thr_id[i] = i;
        if(pthread_create(&pthreads[i], NULL, RunSleep, (void *)&thr_id[i]) != 0)
	    {
            printf("create thread error\n");
            return -1;
        }
        else
            printf("Thread %d was created.\n", i);
    }
    for(int i = 0 ; i < threadnum ; i++)
    {
        pthread_join(pthreads[i], NULL);
    }
}
