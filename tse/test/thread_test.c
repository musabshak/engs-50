
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <pthread.h>    

int x;
pthread_mutex_t m;

// Function that prints the thread nb and sleeps
void *fun(void* argsp) {
    int *thrn =  (int *) argsp;
    printf("Starting Thread %d, waiting %d \n",*thrn,(4-*thrn)*2);
    sleep((4-*thrn)*2);
    printf("================== %d \n", *thrn);
}

// Function that deals with Locking
void *locked_fun(void* argsp){
    pthread_mutex_lock(&m); 
    fun(argsp);
    pthread_mutex_unlock(&m); 
}

// Function that does not deal with locking
void *not_locked_fun(void* argsp){ 
    fun(argsp);
}


void main() {
    // Init
    x = 0;
    pthread_t tid1,tid2,tid3;
    pthread_mutex_init(&m,NULL);

    printf("Starting threads :\n");
    // Do stuff
    int tmp1 = 1;
    if(pthread_create(&tid1,NULL,locked_fun,(void *)&tmp1)!=0)  {
        exit(EXIT_FAILURE);
    }
    int tmp2 = 2;
    if(pthread_create(&tid2,NULL,locked_fun,(void *) &tmp2)!=0)  {
        exit(EXIT_FAILURE);
    }
    int tmp3 = 3;
    if(pthread_create(&tid3,NULL,locked_fun,(void *) &tmp3)!=0)  {
        exit(EXIT_FAILURE);
    }

    // Join 
    if(pthread_join(tid1,NULL)!=0){
        exit(EXIT_FAILURE);
    }
    if(pthread_join(tid2,NULL)!=0){
        exit(EXIT_FAILURE);
    }
    if(pthread_join(tid3,NULL)!=0){
        exit(EXIT_FAILURE);
    }
    
    pthread_mutex_destroy(&m);
    exit(EXIT_SUCCESS); 
}

