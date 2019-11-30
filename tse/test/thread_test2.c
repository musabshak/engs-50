
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <pthread.h>    

pthread_mutex_t m;

void *fxnt2(void* argsp) {
	pthread_mutex_lock(&m);
	printf("fxnt2 just locked the mutex\n");
	printf("fxnt2 doing stuff in thread\n");
	sleep(7);
	pthread_mutex_unlock(&m);
	printf("fxnt2 just unlocked mutex\n");
}


void *fxnt1(void* argsp){
	sleep(3);
	pthread_mutex_lock(&m);
	printf("fxnt1 just locked the mutex\n");
	printf("fxnt1 doing stuff in thread\n");
	sleep(7);
	pthread_mutex_unlock(&m);
	printf("fxnt1 just unlocked the mutex\n");
}

void main() {
    pthread_t tid1, tid2;
    pthread_mutex_init(&m, NULL);

    if(pthread_create(&tid1, NULL, fxnt1, NULL)!=0)  {
        exit(EXIT_FAILURE);
    }

    if(pthread_create(&tid2, NULL, fxnt2, NULL)!=0)  {
        exit(EXIT_FAILURE);
    }

    if(pthread_join(tid1, NULL)!=0){
        exit(EXIT_FAILURE);
    }
    if(pthread_join(tid2, NULL)!=0){
        exit(EXIT_FAILURE);
    }
    
    printf("All threads finished\n");

    pthread_mutex_destroy(&m);
    exit(EXIT_SUCCESS); 
}

