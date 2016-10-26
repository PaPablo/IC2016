#include <stdio.h>
#include <mpi.h>
#include <unistd.h>
#define NUM_STEPS 100000

void calcDeltaX(float *array, unsigned int num_steps){
    float step = 1.0/(float)num_steps;
    int i;
    
    for (i = 0; i < num_steps; i++){
        array[i] = (i + 0.5)*step;
    }
}

float calcIntegralPI(float *array, int num_steps, int size){
    float val = 0.0;
    float x;
    float step = 1.0/(float)(num_steps * size);
    for(int i = 0; i < num_steps; i++){
        x = array[i];
        val += 4.0/(1.0+(x*x));
    }
    
    return val * step;
}

float sumatoria(float *array, int tope){
    float val = 0.0;
    for(int i = 0; i < tope; i++)
        val += array[i];
    
    return val;
}

int main(){
    MPI_Init(NULL, NULL);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    float deltaX[NUM_STEPS];
    //Creamos el arreglo para los valores de X
    if(rank == 0){
        calcDeltaX(deltaX, NUM_STEPS);
        //carga el arreglo con los valores de la abscisa
    }
        
    
    
    int cantidadElementosProceso = NUM_STEPS / size;
    
    float sub_deltaX[cantidadElementosProceso];
    //Creamos un buffer para cada proceso
    
    MPI_Scatter(deltaX, cantidadElementosProceso, MPI_FLOAT,
                sub_deltaX, cantidadElementosProceso, MPI_FLOAT, 
                0, MPI_COMM_WORLD);
    //Asignamos a cada proceso un subarreglo de los valores
    
    float sub_area = calcIntegralPI(sub_deltaX, cantidadElementosProceso, size);
    //calculamos el valor del area debajo de la curva de cada subarreglo
    
    float areas[size];
    
    MPI_Gather(&sub_area, 1, MPI_FLOAT, areas, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
    
    
    if(rank==0){      
        float PI = sumatoria(areas, size);
        printf("PI = %.3f\n", PI);
        
        printf("Probamos de forma secuencial\n");
        PI = calcIntegralPI(deltaX, NUM_STEPS, 1);
        printf("PI (SECUENCIAL) = %.3f\n", PI);
    }
    
    
    
    
    
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
    return 0;
}