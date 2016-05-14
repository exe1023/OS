#define _GNU_SOURCE
#include <stdio.h>
#include <sched.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define threadnum 2

void error(const char *error_msg) 
{
  fprintf(stderr, error_msg);
  exit(EXIT_FAILURE);
}

int SetAffinity(void)
{
  cpu_set_t mask;
  CPU_ZERO(&mask);
  CPU_SET(0, &mask);
  return sched_setaffinity(0, sizeof(mask), &mask);
}
void Busy(void)
{
  for (int i = 0; i < 10000000; i++) {}
}

void *RunSleep(void *num)
{
  int thrnum = (int)num;
  for(int i = 0 ; i < 3 ; i++) {
    printf("Thread %d is running.\n", thrnum);
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
    error("set cpu affinity error\n");
  if(argc > 1)
    if(SetPolicy() != 0)
      error("set policy error\n");
  printf("Sched policy = %d\n" , sched_getscheduler(0));

  pthread_t pthreads[threadnum];
  for(int thr_id = 0 ; thr_id < threadnum ; thr_id++) {
    if(pthread_create(&pthreads[thr_id], NULL, RunSleep, (void *)thr_id) != 0)
      error("create thread error\n");
    else
      printf("Thread %d was created.\n", thr_id);
  }
  for(int i = 0 ; i < threadnum ; i++)
    pthread_join(pthreads[i], NULL);
}
