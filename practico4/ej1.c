#include <mpi.h>
#include <stdio.h>
#include <unistd.h>
_Bool esPar(int val){
    return !(val % 2);
}
int main(int argc, char const *argv[])
{
    int size, rank, rankRecv;
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    if (esPar(rank)) 
    {
        
        printf("RANK %d: Envío %d\n", rank, rank);
        MPI_Send(&rank, 1, MPI_INT, rank+1, 0, MPI_COMM_WORLD);
    } else {        
        MPI_Status status;
        MPI_Recv(&rankRecv, 1, MPI_INT, rank-1, 0, MPI_COMM_WORLD, &status);
        printf("RANK %d: Recibí del proceso RANK %d\n", rank, rankRecv);
        //printf("Msg: %d Source: %d Tag: %d\n", rankRecv, status.MPI_SOURCE, status.MPI_TAG);

    }
    
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
    return 0;
}