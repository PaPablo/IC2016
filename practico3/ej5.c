
#include <stdio.h>
#include <omp.h>

#define MAX 1000


void iniArreglo(int *a){
    int i;
    
    for(i = 0; i < MAX; i++){
        a[i] = i;
    }
    
}

int sumatoria(int *a){
    int valor = 0;
    int i;
    
    //la variable en la que se van a combinar
    //los valores de los resultados de las reducciones
    //se indica en el pragma (valor)
    
    #pragma omp parallel for reduction (+:valor)
    {
        for(i = 0; i < MAX; i++){
            valor += a[i];
        }
    }
    return valor;
}

int main(void){
    int arreglo[MAX];
    
    iniArreglo(arreglo);
    
    int acum = sumatoria(arreglo);
    
    printf("Valor del acumulador: %d\n", (int)acum);
    
    return 0;
}