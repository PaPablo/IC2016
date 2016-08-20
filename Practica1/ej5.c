
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>


#define MAX_CARAC 10
#define MAX_HILOS 2
#define ESPERA 100000



/*Arreglo de hilos*/
pthread_t hilos[MAX_HILOS];

int bandera_hilo = 0;

int indice_global = 0;


char caracAleatorio(void){
    /*Hacemos que el random quede entre 65 y 112 (de la A a la z en ASCII)*/
    return ((rand() % 57) + 65);
}


void *muestraCaracter(void *arreglo){
    int i;
    char carac_a_mostrar;
    
    while(1){
        
        for (i = 0; i < indice_global; i++){
            carac_a_mostrar = * (char *)(arreglo + i);
            printf("%c\n", carac_a_mostrar);
        }
        
        printf("Hilo 2. Mostró %d caracteres\n", i);
        usleep(ESPERA);
        
        if(indice_global > MAX_CARAC)
            pthread_exit((void *) 0);
        
        
        bandera_hilo = 1;        
        while(bandera_hilo);
        
    }
}


void *generaCaracteres (void *arg){
    
    int retval;
    int i;
    
    /*Variables de caracteres*/
    char arregloCarac[MAX_CARAC];
    
    //semilla aleatoria
    srand(getpid());
    
    
    //Creamos hilo consumidor de caracteres
    retval = pthread_create(&hilos[1], NULL, &muestraCaracter, (void *)arregloCarac);
    if(retval != 0)
        exit(1);
            
    //generamos caracteres aleatorios todo el tiempo
    while(1){
        
        indice_global++;
        
        for(i = 0; i < indice_global; i++){
            arregloCarac[i] = caracAleatorio();
        }
        
        if(indice_global > MAX_CARAC)
            pthread_exit((void *) 0);
    
        bandera_hilo = 0;
        while(!bandera_hilo); 
        
    }
    
}

int main(void){
    int retval = pthread_create(&hilos[0], NULL, &generaCaracteres, NULL);
    if(retval != 0)
        exit(1);
    
    pthread_join(hilos[0],NULL);
    
    return 0;
    
}