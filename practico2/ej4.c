
#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>

#define MAX_COLA  1000000
#define MAX_HILOS 10
#define ESPERA    50000

pthread_t hilosEncolar[MAX_HILOS];
pthread_t hilosDesencolar[MAX_HILOS];

typedef struct {
    int final;
    pthread_cond_t llena, vacia;
    pthread_mutex_t mutex;
    int contenido[MAX_COLA];
} cola_t;

cola_t cola = {-1, PTHREAD_COND_INITIALIZER, PTHREAD_COND_INITIALIZER, PTHREAD_MUTEX_INITIALIZER};

int numeroAleatorio(){
    return rand() % 10;
}

void *desencolar(void *args){
    while(1){
        pthread_mutex_lock(&cola.mutex);
        
        if(cola.final == -1){
            printf("COLA_VACIA: espero condicion\n");
            pthread_cond_wait(&cola.vacia, &cola.mutex);
        }
        //mostramos elemento del frente
        printf("%d\n", cola.contenido[0]);
        usleep(ESPERA);
        
        //Realizamos los corrimientos correspondientes
        int i;
        for(i = 1; i <= cola.final; i++){
            cola.contenido[i-1] = cola.contenido[i];
        }
        cola.final--;
        
        pthread_cond_broadcast(&cola.llena);
        pthread_mutex_unlock(&cola.mutex);
    }
    pthread_exit((void *)0);
}


void *encolar(void *args){
    while(1){
        pthread_mutex_lock(&cola.mutex);
        
        if(cola.final == MAX_COLA - 1){
            printf("COLA_LLENA: espero condicion\n");
            pthread_cond_wait(&cola.llena, &cola.mutex);
        }
        
        cola.contenido[++cola.final] = numeroAleatorio();
        
        pthread_cond_broadcast(&cola.vacia);
        pthread_mutex_unlock(&cola.mutex);
    }
    
    pthread_exit((void *)0);
}


int main(void){
    
    int i,
        retval;
    
    for(i = 0; i < MAX_HILOS; i++){
        retval = pthread_create(&hilosEncolar[i], NULL, &encolar, NULL);
        if(retval != 0)
            exit(1);
    }
    
    for(i = 0; i < MAX_HILOS; i++){
        retval = pthread_create(&hilosDesencolar[i], NULL, &desencolar, NULL);
        if(retval != 0)
            exit(1);
    }
    
    
    //como no termina, esperamos un hilo arbitrario
    pthread_join(hilosEncolar[0], NULL);
    
    return 0;
}