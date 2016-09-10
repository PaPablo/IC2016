#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

#define MAX 10
#define ESPERA 50000

pthread_t hilos[MAX];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond= PTHREAD_COND_INITIALIZER;


void *meReBloqueo(void *arg){
    int i = *(int *)arg;
    printf("trato bloquear, hilo %d\n",i);
    pthread_mutex_lock(&mutex);
    //al desbloquearse el mutex en el main, este hilo se despierta
    printf("Me desperté, hilo %d\n", i);
    pthread_mutex_unlock(&mutex);
    
}


int main(void){
    pthread_mutex_lock(&mutex);
    
    int i; 
    for(i = 0; i < MAX; i++){
        //creamos hilos
        int retval = pthread_create(&hilos[i], NULL, &meReBloqueo,(void *)&i);
        if(retval != 0)
            exit(1);
        usleep(ESPERA);
    }
    
    //"despertamos" todos los hilos
    pthread_mutex_unlock(&mutex);
    
    for(i = 0; i < MAX; i++){
        //joineamos todos los hilos
        pthread_join(hilos[i],NULL);    
    }
    return 0;
}