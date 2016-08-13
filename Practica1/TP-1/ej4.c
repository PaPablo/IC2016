
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
    int i;
    
    while(1){
        printf("hilo 2\n");
    }
    /*for(i = 0; i < MAX_CARAC; i++){
        //Mostramos caracter aleatorio si es distinto al anterior mostrado
        if(nuevoCarac != viejoCarac){
            printf("%c\n",nuevoCarac);
            viejoCarac = nuevoCarac;
            usleep(100000);
        }
        pthread_yield();
    }*/
    pthread_exit((void *)0);
}

void *generaCaracteres (void *arg){
    nuevoCarac = caracAleatorio();
    pthread_create(&hilos[1], NULL, &muestraCaracter,NULL);
    while(1){
        printf("hilo 1\n");
    }
    /*int i;
    for(i = 0; i < MAX_CARAC; i++){
        nuevoCarac = saracAleatorio();
        pthread_yield();
        usleep(100000);
    }*/
    pthread_exit((void *)0);
}

int main(void){
    int retval = pthread_create(&hilos[0], NULL, &generaCaracteres, NULL);
    if(retval != 0)
        exit(1);
    
    pthread_join(hilos[0],NULL);
    
    return 0;
    
}