#include <stdio.h>
#include <mpi.h>
#define NUM_STEPS 100000
#define MAX_PROC 10

void calcDeltaX(float *array, unsigned int num_steps){
    float incremento = (float)num_steps/(num_steps * 2); 
    float step = 1.0/(float)num_steps;
    
    for (int i = 0; i < num_steps; i++)
        array[i] = (i + incremento)*step;
}

float calcIntegralPI(float *array, int num_steps){
    float val = 0.0;
    float x;
    for(int i = 0; i < num_steps; i++){
        x = array[i];
        val += 4.0/(1.0+(x*x));
    }
    
    return val;
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
    if(rank == 0)
        calcDeltaX(deltaX, NUM_STEPS);
    //carga el arreglo con los valores de la abscisa
    
    
    
    float sub_deltaX[NUM_STEPS/MAX_PROC];
    //Creamos un buffer para cada proceso
    
    MPI_Scatter(deltaX, NUM_STEPS/MAX_PROC, MPI_FLOAT, sub_deltaX, NUM_STEPS/MAX_PROC, MPI_FLOAT, 0, MPI_COMM_WORLD);
    //Asignamos a cada proceso un subarreglo de los valores
    
    float sub_area = calcIntegralPI(sub_deltaX, NUM_STEPS/MAX_PROC);
    //calculamos el valor del area debajo de la curva de cada subarreglo
    
    float areas[MAX_PROC];
    
    MPI_Gather(&sub_area, 1, MPI_FLOAT, areas, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
    
    
    if(rank==0){
        float PI = sumatoria(areas, MAX_PROC) / NUM_STEPS;
        printf("PI = %.5f\n", PI);
    }
    
    
    
    
    
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
    return 0;
}