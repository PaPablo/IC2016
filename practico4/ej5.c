#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <unistd.h>


typedef struct{
    int u, v;
} Dupla;

/*

u = (u1, u2, ... , ui, ... , un)
v = (v1, v2, ... , vi, ... , vn)

*/

void init_vector(Dupla *dupla, int dim){
    int result = 0;
    int i;
    printf("u = (");
    for(i = 0; i < dim; i++){
        dupla[i].u = rand()%10;
        printf("%d, ", dupla[i].u);
    }
    
    printf(")\nv = (");
    
    for(i = 0; i < dim; i++){
        dupla[i].v = rand()%10;
        printf("%d, ", dupla[i].v);
    }
    printf(")\n\n");
}

int sumatoria(int *array, int tope){
    int val = 0;
    for(int i = 0; i < tope; i++)
        val += array[i];
    
    return val;
}

int dot(Dupla *dupla, int dim){
    int val = 0;
    int u,v;
    for(int i = 0; i < dim; i++){
        u = dupla[i].u;
        v = dupla[i].v;
        printf("u%d = %d, v%d = %d\n", i, dupla[i].u, i, dupla[i].v);
        val += dupla[i].u * dupla[i].v;
    }
    printf("dot = %d\n\n", val);
    return val;
}



int main(){
    srand(getpid());
    MPI_Init(NULL, NULL);    
    
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if(size < 1){
        printf("Ingrese valor de procesos mayor a 1\n");
        exit(1);
    }
        
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    /*Crear dos vectores de (size) dimensiones
    a cada proceso le doy un Dupla (xi,yi) Duplaa que calcule
    los juntamos, sumamos y tenemos el producto punto de dos vectores
    */

    int vectors_size = size*2;
    
    Dupla vectors[vectors_size];
    //creamos dos vectores de (size*2) dimensiones
    
    
    if(rank == 0){
        init_vector(vectors, vectors_size);
        /*
        for(int i = 0; i < vectors_size; i++){
            printf("u%d = %d\nv%d = %d\n\n", i, vectors[i].u, i, vectors[i].v);
        }
        printf("FIN INICIALIZACIÓN\n\n");*/
    }
    
    
    int sub_vector_size = vectors_size / size;

    
    Dupla sub_vec[sub_vector_size];

    
    
    MPI_Scatter(vectors, sub_vector_size * 2, MPI_INT, 
                sub_vec, sub_vector_size * 2, MPI_INT, 
                0, MPI_COMM_WORLD);
    
    
    int sub_dot = dot(sub_vec, sub_vector_size);
    
    int dot_product[size];
    
    
    MPI_Gather(&sub_dot, 1, MPI_INT, dot_product, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    if(rank == 0){
        int dot_result = sumatoria(dot_product, size);
        printf("Resultado final = %d\n", dot_result);
        
        printf("\nProbamos de forma secuencial\n");
        int dot_sec = dot(vectors, vectors_size);
        printf("Resultado final (SECUENCIAL) = %d\n", dot_sec);
    }
    
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
    return 0;
}