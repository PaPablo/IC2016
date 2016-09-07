#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>

#define MAX_PILA  100000
#define MAX_HILOS 10
#define ESPERA    50000
#define CORTE_ITERACIONES 1000
pthread_t hilosPush[MAX_HILOS];
pthread_t hilosPop[MAX_HILOS];

typedef struct {
    int tope;
    pthread_cond_t llena, vacia;
    pthread_mutex_t mutex;
    int contenido[MAX_PILA];
} pila_t;

void crearPila(pila_t *pila){

    pila->tope  = (-1);
    pthread_cond_init(&pila->llena,NULL);
    pthread_cond_init(&pila->vacia,NULL);
    pthread_mutex_init(&pila->mutex,NULL);
}

void push(pila_t *pila, int elemento){
    pthread_mutex_lock(&pila->mutex);

    if(pila->tope == (MAX_PILA-1)){
        printf("PILA_LLENA: Espero condicion\n");
        pthread_cond_wait(&pila->llena,&pila->mutex);
    }
    //el tope apunta al elemento visible de la pila
    pila->tope += 1;
    pila->contenido[pila->tope] = elemento;

    pthread_cond_signal(&pila->vacia);

    pthread_mutex_unlock(&pila->mutex);
}

int pop(pila_t *pila){
    pthread_mutex_lock(&pila->mutex);

    if(pila->tope == -1){
        printf("PILA_VACIA: Espero condicion\n");
        pthread_cond_wait(&pila->vacia,&pila->mutex);
    }
    //el tope apunta al elemento visible de la pila
    int valor = pila->contenido[pila->tope];
    pila->tope -= 1;    
    pthread_cond_signal(&pila->llena);
    
    pthread_mutex_unlock(&pila->mutex);

    return valor;
}



int numeroAleatorio(){
    return rand() % 10;
}

void *hiloPush (void *arg){
    pila_t *p = (pila_t *)arg;
    int corte = 0;
    while(1){
        push(p, numeroAleatorio());
        corte++;
    }

    pthread_exit((void *)0);
}

void *hiloPop (void *arg){
    pila_t *p = (pila_t *)arg;
    int corte = 0;
    while(1){
        printf("%d\n",pop(p));
        corte++;
    }
    //pop desde la pila
    //si se vacia, espera condicion
    
    pthread_exit((void *)0);
}
int main(void){
    
    pila_t pila;

    crearPila(&pila);
    int i;
    int retval;
    srand(getpid());
    
    
    
    //hilos popeadores
    for(i = 0; i <  MAX_HILOS ; i++){
        retval = pthread_create(&hilosPop[i],NULL,&hiloPop, (void *)&pila);
        if(retval != 0)
            exit(1);
    }
    
    //hilos pusheadores
    for(i = 0; i < MAX_HILOS ; i++){
        retval = pthread_create(&hilosPush[i],NULL,&hiloPush, (void *)&pila);
        if(retval != 0)
            exit(1);
    }
    
    
    //esperamos al primer hilo creado
    pthread_join(hilosPush[0], NULL);

    return 0;
}