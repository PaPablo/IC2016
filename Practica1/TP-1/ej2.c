
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

#define MAX_ARREGLO 20
#define MAX_HILOS 2

/*Declaramos arreglo de enteros*/
int arreglo[MAX_ARREGLO];
    
void *recorrerArreglo(void *arg){ 
    int i;
    int valor;
    
    /* Recorremos el arreglo y mostramos valores*/
    for(i = 0; i < MAX_ARREGLO; i++){
        printf("El valor del arreglo en la posicion %d es: %d\n", i, arreglo[i]);
        usleep(100000);
        
    }
    
    pthread_exit((void *)0);
}

void inicializarArreglo(int *arrg, int tope){
    /*Armamos random seed*/
    srand(getpid());
    
    int i; 
    
    for(i = 0; i < tope; i++){
        arrg[i] = rand();
    }
}
int main(void){
    
    int retval = 0;
    
    /*Declaramos arreglo de hilos*/
    pthread_t hilos[MAX_HILOS];
    
    inicializarArreglo(arreglo, MAX_ARREGLO);
    
    int i;
    for(i = 0; i < MAX_HILOS; i++){
        /*Creamos hilo*/
        printf("Hilo %d\n",i);
        retval = pthread_create(&hilos[i],NULL, &recorrerArreglo, NULL);
        if(retval != 0){
            exit(1);
        }
        pthread_join(hilos[i], NULL);
    }
       
    return 0;
}