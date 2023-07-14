#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>

void time_printA()
{
        struct timeval tv; 
        gettimeofday(&tv,NULL);
        printf("threadA microsecond:%ld\n",tv.tv_sec*1000000 + tv.tv_usec);  //微秒
}
void time_printB()
{
        struct timeval tv; 
        gettimeofday(&tv,NULL);
        printf("threadB microsecond:%ld\n",tv.tv_sec*1000000 + tv.tv_usec);  //微秒
}

void *funA(void *arg)
{
        printf("I'm thread, Thread ID = %lu\n", pthread_self());
        while(1)
        {   
                time_printA();
                usleep (1000);
               
        } 
        return NULL;
}

void *funB(void *arg)
{
        printf("I'm thread 1, Thread ID = %lu\n", pthread_self());
        while(1)
        {   
                time_printB();
                usleep(1000);
        }   
        return NULL;
}

//如果想运行将cpuTest_main改为main函数即可
int cpuTest_main()
{
        pthread_t tid;
        pthread_t tid1;

        pthread_create(&tid, NULL, funA, NULL);
        pthread_create(&tid, NULL, funB, NULL);

        void *status;
        pthread_join(tid, &status);

        return 0;
}
