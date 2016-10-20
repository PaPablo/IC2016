#include <mpi.h>
#include <stdio.h>
#include <unistd.h>
_Bool esPar(int val){
    return !(val % 2);
}
int main(int argc, char const *argv[])
{
    int size, rank;
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int x;
    
    if (esPar(rank)) 
    {
        
        printf("RANK %d: Envio %d\n", rank, x);
        MPI_Send(&x, 1, MPI_INT, rank+1, 0, MPI_COMM_WORLD);
    } else {
        MPI_Status status;
        MPI_Recv(&x, 1, MPI_INT, rank-1, 0, MPI_COMM_WORLD, &status);
        printf("RANK %d: Recibi %d\n", rank, x);
        //printf("Msg: %d Source: %d Tag: %d\n", x, status.MPI_SOURCE, status.MPI_TAG);

    }
    
    
    MPI_Finalize();
    return 0;
}