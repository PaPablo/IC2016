
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

#define _GNU_SOURCE

#define MAX_CARAC 20
#define MAX_HILOS 2

/*Variables de caracteres*/
char nuevoCarac, viejoCarac = 0;

/*Arreglo de hilos*/
pthread_t hilos[MAX_HILOS];


char caracAleatorio(){
    srand(getpid());
    /*Hacemos que el random quede entre 65 y 112*/
    return ((rand() % 57) + 65);
}

void *muestraCaracter(void *arg){
    //Mostramos caracter aleatorio si es distinto al anterior mostrado
    while(1){
        if(nuevoCarac != viejoCarac){
            
        printf("%c\n",nuevoCarac);
        usleep(600000);
        viejoCarac = nuevoCarac;        
        }
    }
    pthread_exit((void *)0);
}

void *generaCaracteres (void *arg){
    int retval;
    
    //semilla aleatoria
    srand(getpid());
    
    //generamos caracter aleatorio
    nuevoCarac = caracAleatorio();
    
    //Creamos hilo consumidor de caracteres
    retval = pthread_create(&hilos[1], NULL, &muestraCaracter,NULL);
    if(retval != 0)
        exit(1);
            
    //generamos caracteres aleatorios todo el tiempo
    while(1){
        if(viejoCarac == nuevoCarac){
            /*Hacemos que el random quede entre 65 y 112*/
            nuevoCarac = ((rand() % 57) + 65);
        }
    }
    
    pthread_exit((void *)0);
}

int main(void){
    int retval = pthread_create(&hilos[0], NULL, &generaCaracteres, NULL);
    if(retval != 0)
        exit(1);
    
    pthread_join(hilos[0],NULL);
    
    return 0;
    
}