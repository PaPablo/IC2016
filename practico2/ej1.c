#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

#define MAX 3

pthread_t hilos[MAX];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int contadorSegundoHilo = 0;


void *tercerHilo(void *arg){
    int i;
    pthread_mutex_lock(&mutex);
    for(i = 0; i < 10 ; i++){
        usleep(500000);
        printf("%d %d\n", contadorSegundoHilo, i);
    }
    pthread_mutex_unlock(&mutex);
}

void *segundoHilo(void *contador){


    
    pthread_create(&hilos[2], NULL, &tercerHilo, NULL);
    while(contadorSegundoHilo<10){
        pthread_mutex_lock(&mutex);
        contadorSegundoHilo++;
        pthread_mutex_unlock(&mutex);
    }
}

void *primerHilo(void *arg){
    int contadorSegundoHilo = 0;
    int retval = pthread_create(&hilos[1], NULL, &segundoHilo,NULL);
    if(retval != 0)
        exit(1);
    
    while(contadorSegundoHilo < 10);
    
    pthread_exit((void *)0);
}





int main(void){
    
    //primer hilo
    int retval = pthread_create(&hilos[0], NULL, &primerHilo,NULL);
    if(retval != 0)
        exit(1);
    
    pthread_join(hilos[0],NULL);
    
    
    return 0;
}