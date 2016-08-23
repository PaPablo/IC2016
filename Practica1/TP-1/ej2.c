
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

#define MAX_ARREGLO 20

void inicializarArreglo(int *arrg, int tope){
    /*Armamos random seed*/
    srand(getpid());
    
    int i; 
    
    for(i = 0; i < tope; i++){
        arrg[i] = rand();
    }
}
int main(void){
    
    /*Declarar arreglo*/
    int arreglo[MAX_ARREGLO];
    
    
    inicializarArreglo(arreglo, MAX_ARREGLO);
    
    int i;
    
    /* Recorremos el arreglo y mostramos valores*/
    for(i = 0; i < MAX_ARREGLO; i++){
        printf("El valor del arreglo en la posicion %d es: %d\n", i, arreglo[i]);
        usleep(500000);
    }
    
    return 0;
}