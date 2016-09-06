#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>

#define MAX_PILA  1000
#define MAX_HILOS 10
#define ESPERA    50000

pthread_t hilosPush[MAX_HILOS];
pthread_t hilosPop[MAX_HILOS];

typedef struct {
    int tope;
    pthread_cond_t llena, vacia;
    pthread_mutex_t mutex;
    int contenido[MAX_PILA];
} pila_t;

pila_t pila  = {-1, PTHREAD_COND_INITIALIZER, PTHREAD_COND_INITIALIZER, PTHREAD_MUTEX_INITIALIZER};
    
int numeroAleatorio(){
    return rand() % 10;
}
void *push (void *arg){
    while(1){
        pthread_mutex_lock(&pila.mutex);
        if(pila.tope == (MAX_PILA-1)){
            pthread_cond_wait(&pila.llena,&pila.mutex);
            printf("PILA_LLENA: Espero condicion\n");
        }
        //el tope apunta al elemento visible de la pila
        pila.contenido[++pila.tope] = numeroAleatorio();
        
        pthread_cond_broadcast(&pila.vacia);
        
        pthread_mutex_unlock(&pila.mutex);
        
    }
    pthread_exit((void *)0);
}

void *pop (void *arg){
    while(1){
        pthread_mutex_lock(&pila.mutex);
        if(pila.tope == -1){
            pthread_cond_wait(&pila.vacia,&pila.mutex);
            printf("PILA_VACIA: Espero condicion\n");
        }
        //el tope apunta al elemento visible de la pila
        printf("%d\n", pila.contenido[pila.tope]);
        usleep(ESPERA);
        pila.tope--;
        
        pthread_cond_broadcast(&pila.llena);
        
        pthread_mutex_unlock(&pila.mutex);
        
    }
    //pop desde la pila
    //si se vacia, espera condicion
    
    pthread_exit((void *)0);
}
int main(void){
    
    int i;
    int retval;
    srand(getpid());
    
    
    //hilos pusheadores
    for(i = 0; i < MAX_HILOS ; i++){
        retval = pthread_create(&hilosPush[i],NULL,&push, NULL);
        if(retval != 0)
            exit(1);
    }
    
    sleep(1);
    
    //hilos popeadores
    for(i = 0; i < MAX_HILOS ; i++){
        retval = pthread_create(&hilosPop[i],NULL,&pop, NULL);
        if(retval != 0)
            exit(1);
    }
    
    
    //esperamos al primer hilo creado
    pthread_join(hilosPush[0], NULL);
    return 0;
}