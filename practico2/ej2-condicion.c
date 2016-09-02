#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

#define MAX 10
#define ESPERA 500

pthread_t hilos[MAX];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond= PTHREAD_COND_INITIALIZER;


void *esperoCondicion(void *arg){
    int i = (int)arg;
    pthread_cond_wait(&cond,&mutex);
    printf("Me desperté, hilo %d\n", i);
    pthread_mutex_unlock(&mutex);
    
}


int main(void){
    pthread_mutex_lock(&mutex);
    int i; 
    for(i = 0; i < MAX; i++){
        //creamos hilos
        int retval = pthread_create(&hilos[i], NULL, &esperoCondicion,(void *)i);
        if(retval != 0)
            exit(1);     
    }
    
    pthread_cond_signal(&cond);
 
    
    for(i = 0; i < MAX; i++){
        //joineamos todos los hilos
        pthread_join(hilos[i],NULL);    
    }
    return 0;
}