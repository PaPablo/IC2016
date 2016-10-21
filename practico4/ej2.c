#include <mpi.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

#define MAX_PROCESOS 3

_Bool esPar(int val){
    return !(val % 2);
}

int main(){
    int size, rank;
    srand(getpid());
    
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    if(rank == 0){
        
        int envio;
        int mjs = rand()%100;
        
        printf("Soy el RANK %d, envío\n", rank);
        
        for(int i = 0; i < mjs; i++){
            envio = rand() % 100;
            MPI_Send(&envio, 1, MPI_INT, rand()%2, rand()%100, MPI_COMM_WORLD);
            //enviamos un mensaje con un valor y etiqueta aleatoria
        }
    } else if (rank == 1){
        
        //los demas procesos reciben
        
        MPI_Status status;
        int msj;
        
        MPI_Recv(&msj, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        //recibimos el mensaje de cualquier fuente y con cualquier etiqueta
        
        if(esPar(status.MPI_TAG))
            printf("RANK %d, recibí un %d (SOURCE %d, TAG %d)\n", rank, msj, status.MPI_SOURCE, status.MPI_TAG);
    } else{
        MPI_Status status;
        int msj;
        
        MPI_Recv(&msj, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        //recibimos el mensaje de cualquier fuente y con cualquier etiqueta
        
        if(!esPar(status.MPI_TAG))
            printf("RANK %d, recibí un %d (SOURCE %d, TAG %d)\n", rank, msj, status.MPI_SOURCE, status.MPI_TAG);
    }
    
        
    
    
    MPI_Finalize();
    return 0;
}