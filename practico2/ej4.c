
#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>

#define MAX_COLA  100
#define MAX_HILOS 10
#define MAX_ITERACIONES	10000
#define ESPERA    50000

pthread_t hilosEncolar[MAX_HILOS];
pthread_t hilosDesencolar[MAX_HILOS];

typedef struct {
    int final;
    pthread_cond_t llena, vacia;
    pthread_mutex_t mutex;
    int contenido[MAX_COLA];
} cola_t;

int numeroAleatorio(){
    return rand() % 10;
}

void crearCola(cola_t *cola){
	cola->final = -1;
	pthread_cond_init(&cola->llena, NULL);
	pthread_cond_init(&cola->vacia, NULL);
	pthread_mutex_init(&cola->mutex, NULL);

}

void desencolar(cola_t *cola, int *frente){
	pthread_mutex_lock(&cola->mutex);
	
	if(cola->final == -1){
		printf("COLA_VACIA: espero condicion\n");
		pthread_cond_wait(&cola->vacia, &cola->mutex);
	}
	//recuperamos elemento del frente
	*frente = cola->contenido[0];
	
	//Realizamos los corrimientos correspondientes
	int i;
	for(i = 1; i <= cola->final; i++){
		cola->contenido[i-1] = cola->contenido[i];
	}
	cola->final -= 1;
	
	pthread_cond_signal(&cola->llena);
	pthread_mutex_unlock(&cola->mutex);
	
}


void encolar(cola_t *cola, int valor){
    
	pthread_mutex_lock(&cola->mutex);
	
	if(cola->final == MAX_COLA - 1){
		printf("COLA_LLENA: espero condicion\n");
		pthread_cond_wait(&cola->llena, &cola->mutex);
	}
	//insertamos el valor pasado por parametro en la ultima posicion de la cola
	cola->final += 1;
	cola->contenido[cola->final] = valor;
		
	pthread_cond_signal(&cola->vacia);
	pthread_mutex_unlock(&cola->mutex);
    
}


void *funcionHiloDesencola(void *cola){
	cola_t *q = (cola_t *)cola;
	int frente,
		corte = 0;
	while(corte < MAX_ITERACIONES){
		desencolar(q, &frente);
		printf("%d\n", frente);
		usleep(ESPERA);
		corte++;
	}
	
	pthread_exit((void *)0);

}


void *funcionHiloEncola(void *cola){
	cola_t *q = (cola_t*)cola;
	int corte = 0;
	while(corte < MAX_ITERACIONES){
		encolar(q, numeroAleatorio());
		corte++;
	}
	
	pthread_exit((void *)0);
}


int main(void){
    
    int i,
        retval;
        
    cola_t q;
    crearCola(&q);
    srand(getpid());
    
    
    /*
    for(i = 0; i < 11; i++){
		retval = numeroAleatorio();
		printf("voy a encolar un %d. Iteracion numero %d\n", retval, i);
		encolar(&q, retval);
	}*/
   
    
    
    for(i = 0; i < MAX_HILOS; i++){
        retval = pthread_create(&hilosEncolar[i], NULL, &funcionHiloEncola, (void *)&q);
        if(retval != 0)
            exit(1);
    }
    
    for(i = 0; i < MAX_HILOS; i++){
        retval = pthread_create(&hilosDesencolar[i], NULL, &funcionHiloDesencola, (void *)&q);
        if(retval != 0)
            exit(1);
    }
    
    
    //esperamos hilos
    for(i = 0; i < MAX_HILOS; i++){
        pthread_join(hilosEncolar[i], NULL);
        pthread_join(hilosDesencolar[i], NULL);
    }
    
    return 0;
}
