
#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>

#define MAX_HILOS	10
#define ESPERA 		50000

typedef struct 
{
	unsigned char 	valor;
	pthread_mutex_t mutex;
	pthread_cond_t 	esperoPar;
} numero_t;


numero_t arreglo[MAX_HILOS/2];
pthread_t hilos[MAX_HILOS];


int numeroAleatorio(int rango){
	return ((rand()) % rango);
}

void arregloAleatorio(){
	int i;
	for(i = 0; i < (MAX_HILOS/2); i++){
		arreglo[i].valor 	= (unsigned char)numeroAleatorio(100);
		//arreglo[i].tomado 	= 0;
		pthread_mutex_init(&arreglo[i].mutex, NULL);
		pthread_cond_init(&arreglo[i].esperoPar, NULL);
	}
}


void *buscoHiloPar(void *args){
	
	int numeroHilo = *(int *)args;
	int posicion = numeroHilo % (MAX_HILOS / 2);
	printf("Hilo %d tiene número %d.\n", numeroHilo, arreglo[posicion].valor);


	/*Enviamos señal. Si es el primer hilo que tiene el número, la señal se pierde.
	Si es el par el que envía la señal, la recibe el primer hilo y sale de la espera*/
	pthread_cond_signal(&arreglo[posicion].esperoPar);

	/*Lockeamos mutex. El primer hilo espera señal del par 
	y el par espera la respuesta del primero*/
	pthread_mutex_lock(&arreglo[posicion].mutex);
	pthread_cond_wait(&arreglo[posicion].esperoPar, &arreglo[posicion].mutex);

	/*Unlockeamos mutex para salir de zona crítica y despertar algún hilo dormido.
	Primer hilo envía respuesta al par*/
	pthread_mutex_unlock(&arreglo[posicion].mutex);
	pthread_cond_signal(&arreglo[posicion].esperoPar);

	printf("El número %d está libre (Hilo %d)\n", arreglo[posicion].valor, numeroHilo);	

	pthread_exit((void *) 0);
}


int main (void){

	srand(getpid());
	arregloAleatorio();

	int i;
	for(i = 0; i < MAX_HILOS; i++){
		pthread_create(&hilos[i], NULL, &buscoHiloPar, (void *)&i);
		usleep(ESPERA * 5);
	}

	for(i = 0; i < MAX_HILOS; i++){
		pthread_join(hilos[i], NULL);
	}


	return 0;
}