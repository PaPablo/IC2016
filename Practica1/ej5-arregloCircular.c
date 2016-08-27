

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>


#define MAX_CARAC 10
#define MAX_HILOS 2
#define ESPERA 500000

int prod = 0;
int cons = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


char caracteres[MAX_CARAC];
pthread_t hilos[MAX_HILOS];

int v = 1;
char caracAleatorio(void){
    /*Hacemos que el random quede entre 65 y 112 (de la A a la z en ASCII)*/
    return ((rand() % 57) + 65);
}

/*Arreglo de hilos*/
pthread_t hilos[MAX_HILOS];

void *consumidor(void *arreglo){
    
    
    while(1){
        //bloqueo
        pthread_mutex_lock(&mutex);
        //consumo hasta alcanzar al indice de productor
        while(prod != cons){
            printf("%d, %d, %c\n",prod, cons, caracteres[cons]);
            usleep(ESPERA);
            
            //manejo del arreglo circular
            cons++;
            if (cons == MAX_CARAC){
                cons = 0;
            }
        }
        //desbloqueo
        pthread_mutex_unlock(&mutex);
        
        //espero a que se produzcan mas
        while(prod==cons);
        

    }
    
    pthread_exit((void *)0);
}


void *generador (void *arg){
    
    //creamos hilo consumidor
    int retval = pthread_create(&hilos[1], NULL, &consumidor, NULL);
    if(retval != 0)
        exit(1);
    
    //repetimos infinitamente
    while(1){
        
        /*si el indice productor esta ahi no mas del consumidor
        esperamos a que se consuman más para seguir produciendo*/
        if (prod != (cons-1)){
            
            //genero caracter
            caracteres[prod] = caracAleatorio();
            
            //manejo del arreglo circular
            prod++;
            if(prod == MAX_CARAC){
                prod = 0;
            } 
            
        } else {
            //espero a que se consuman
            while(prod == (cons-1));
        }
    }
    
    pthread_exit((void *)0);
    
    
}

int main(void){
    
    
    srand(getpid());
    
    //creamos el hilo generador
    int retval = pthread_create(&hilos[0], NULL, &generador, NULL);
    if(retval != 0)
        exit(1);
    
    pthread_join(hilos[0],NULL);
    
    return 0;   
}