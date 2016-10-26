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
        
        printf("RANK %d: Envío %d al RANK %d\n", rank, rank, rank+1);
        MPI_Send(&rank, 1, MPI_INT, rank+1, 0, MPI_COMM_WORLD);
    } else {        
        MPI_Status status;
        MPI_Recv(&rankRecv, 1, MPI_INT, rank-1, 0, MPI_COMM_WORLD, &status);
        printf("RANK %d: Recibí %d del proceso RANK %d\n", rank, rankRecv, rankRecv);

    }
    
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
    return 0;
}