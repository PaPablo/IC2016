
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>


#define MAX_CARAC 20
#define MAX_HILOS 2


/*BUCLE INFINITO. SE PREGUNTA SI EL CONTADOR LLEGO AL TOPE.
SI LLEGO, SE TERMINA EL HILO*/


/*Variables de caracteres*/
int retorno;
char nuevoCarac, viejoCarac = 0;

/*Arreglo de hilos*/
pthread_t hilos[MAX_HILOS];

int contador = 0;

char caracAleatorio(void){
    /*Hacemos que el random quede entre 65 y 112 (de la A a la z en ASCII)*/
    return ((rand() % 57) + 65);
}


void *muestraCaracter(void *arg){
    //Mostramos caracter aleatorio si es distinto al anterior mostrado
    
    while(1){
        if(contador >= MAX_CARAC){
            pthread_exit((void *) 0);
        }
        else if(nuevoCarac != viejoCarac){
            printf("%c\n",nuevoCarac);
            usleep(500000);
            viejoCarac = nuevoCarac;
            contador++;

            //retorno = sched_yield();
        }
    }
    
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
        nuevoCarac = caracAleatorio();
        if (contador >= MAX_CARAC){
            pthread_exit((void *) 0);
        }
      
        
        /*el pthread_yield no quiere andar, tira warning de que no esta declarada
        con el sched, realmente es lo mismo que nada. No se si se lo estara usando mal,
        pero si no esta funciona mejor el programa.
        Los hilos switchean solos, como en Round Robin.
        retorno = sched_yield();*/
    }
    
    
}

int main(void){
    int retval = pthread_create(&hilos[0], NULL, &generaCaracteres, NULL);
    if(retval != 0)
        exit(1);
    
    pthread_join(hilos[0],NULL);
    
    //De onda
    printf("\n");
    printf("caracteres impresos: %d\n", contador);
    
    return 0;
    
}