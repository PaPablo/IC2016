#include <mpi.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main(){
    int size, rank;
    srand(getpid());
    
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    if((rank == 0) || (rank == 1) || (rank == 2)){
        //establecemos que los procesos con rank 0, 1, 2 van a ser lo que envien mensajes
        
        int envio;
        
        printf("Soy el RANK %d, envío\n", rank);
        
        for(int i = rank+1; i < size; i++){
            envio = rand() % 100;
            MPI_Send(&envio, 1, MPI_INT, i, (rand()%100)+3, MPI_COMM_WORLD);
            //enviamos un mensaje con un valor y etiqueta aleatoria
        }
    }
    else{
        //los demas procesos reciben
        
        MPI_Status status;
        int msj;
        
        MPI_Recv(&msj, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        //recibimos el mensaje de cualquier fuente y con cualquier etiqueta
        
        printf("RANK %d, recibí un %d (SOURCE %d, TAG %d)\n", rank, msj, status.MPI_SOURCE, status.MPI_TAG);
        
    }
    
        
    
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
    return 0;
}