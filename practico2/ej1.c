#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

#define MAX 3
#define ESPERA 500

pthread_t hilos[MAX];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond= PTHREAD_COND_INITIALIZER;

int contadorSegundoHilo = 0;


int esperaAleatoria(){
    return (((rand()%4)+1) * 100) * 1000;
}

void *tercerHilo(void *arg){
    
    
    int i;

    for(i = 1; i <= 100; i++){
        
        if (!(i%10)){
            /*
                Si el contador de este hilo llego a diez
                despierta al segundo hilo 
            */
            pthread_cond_signal(&cond);
        }
        
        printf(" %d %d\n", contadorSegundoHilo, i%10);
        usleep(esperaAleatoria());
    }
    
   pthread_exit((void *)0);
}

void *segundoHilo(void *contador){

    pthread_create(&hilos[2], NULL, &tercerHilo, NULL);
    while(contadorSegundoHilo<10){
        
        pthread_mutex_lock(&mutex);

        //esperamos contador interno del tercer hilo
        pthread_cond_wait(&cond,&mutex);
        //incrementamos
        contadorSegundoHilo++;

        pthread_mutex_unlock(&mutex);
        
    }
    
    pthread_exit((void *)0);
    
}

void *primerHilo(void *arg){
    int retval = pthread_create(&hilos[1], NULL, &segundoHilo,NULL);
    if(retval != 0)
        exit(1);
    
    //nos quedamos esperando al segundo hilo
    while(contadorSegundoHilo < 10);
    
    pthread_exit((void *)0);
}





int main(void){
    
    //generacion de random seed
    srand(getpid());
    
    //primer hilo
    int retval = pthread_create(&hilos[0], NULL, &primerHilo,NULL);
    if(retval != 0)
        exit(1);
    
    //joineamos al primer hilo
    pthread_join(hilos[0],NULL);
    
    return 0;
}