#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>

#define MAX_PILA            100
#define MAX_HILOS           10
#define CORTE_ITERACIONES   10

#define ESPERA              50000

pthread_t hilosPush[MAX_HILOS];
pthread_t hilosPop[MAX_HILOS];

typedef struct {
    int tope;
    pthread_cond_t llena, vacia;
    pthread_mutex_t mutex;
    int contenido[MAX_PILA];
} pila_t;



/***********************************************************/
/*Tanto en este como en el ej4 (implentación de colas)      /
/*puede pinchar (segmentation fault o stack smashed)        /
/*por lo distintos valores de las constantes, más que nada  /
/*por el CORTE_ITERACIONES.                                 /
/*Ir incrementado o decrementando para ver si pincha o no   /
/***********************************************************/



void crearPila(pila_t *pila){

    pila->tope  = -1;
    pthread_cond_init(&pila->llena,NULL);
    pthread_cond_init(&pila->vacia,NULL);
    pthread_mutex_init(&pila->mutex,NULL);
}


void push(pila_t *pila, int elemento){
    pthread_mutex_lock(&pila->mutex);

    if(pila->tope == (MAX_PILA-1)){
        printf("PILA_LLENA: Espero condicion\n");
        pthread_cond_wait(&pila->llena, &pila->mutex);
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

//funcion hilosPusheadores
void *hiloPush (void *arg){
    pila_t *p = (pila_t *)arg;
    int corte = 0;
    while(corte < CORTE_ITERACIONES){
        push(p, numeroAleatorio());
        corte++;
    }
    pthread_exit((void *)0);
}

//funcion hilos popeadores
void *hiloPop (void *arg){
    pila_t *p = (pila_t *)arg;
    int corte = 0;
    while(corte < CORTE_ITERACIONES){
        printf("%d\n",pop(p));
        usleep(ESPERA);
        corte++;
    }
    pthread_exit((void *)0);
}


int main(void){
    
    pila_t pila;

    int i,
        retval;

    crearPila(&pila);
    srand(getpid());
    
    
    //hilos pusheadores
    for(i = 0; i < MAX_HILOS ; i++){
        retval = pthread_create(&hilosPush[i], NULL, &hiloPush, (void *)&pila);
        if(retval != 0)
            exit(1);
    }
    
    //hilos popeadores
    for(i = 0; i <  MAX_HILOS ; i++){
        retval = pthread_create(&hilosPop[i], NULL, &hiloPop, (void *)&pila);
        if(retval != 0)
            exit(1);
    }
    
    
    //esperamos hilos popeadores
    for(i = 0; i <  MAX_HILOS ; i++){
        pthread_join(hilosPop[i],NULL);
    }
    
    //esperamos hilos pusheadores
    for(i = 0; i < MAX_HILOS ; i++){
        pthread_join(hilosPush[i],NULL);
    }

    return 0;
}
