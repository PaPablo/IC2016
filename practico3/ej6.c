
#include <stdio.h>
#include <omp.h>
#include <unistd.h>

#define MAX 1000


void iniArreglo(int *a){
    int i;
    for(i = 0; i < MAX; i++){
        a[i] = i;
    }
}

int sumatoria(int *a){
    int valor;
    int i;
    
    #pragma omp parallel for reduction(+:valor)
    for(i = 0; i < MAX; i++){
        valor += a[i];
        usleep(10);  
        /*agregamos espera para que se note la diferencia
        entre la ejecución paralela y secuencial*/
    }
    return valor;
}

int main(void){
    int arreglo[MAX];
    
    iniArreglo(arreglo);
    
    int acum = sumatoria(arreglo);
    
    printf("Valor del acumulador: %d\n", acum);
    
    return 0;
}