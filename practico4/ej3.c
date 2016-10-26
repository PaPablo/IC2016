#include <mpi.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

#define TAG_EXIT 0

/*Hay un problema con el Probe. Si se hace el Probe del mensaje pero no se recibe,
se queda haciendo el Probe con ese mensaje y no recibe los nuevos (no desencola el mensaje)
Habría que ver una forma para que pueda descartar ese mensaje que no le sirve*/ 

_Bool esPar(int val){
    return !(val % 2);
}

int main(){
    int size, rank;
    int mensaje;
    MPI_Status status;
    
    srand(getpid());
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    if(rank == 0){
        
        int envio, i;
        int msj = (rand()%100)+1;
        
        printf("Soy el RANK %d, voy a enviar %d mensajes\n", rank, msj);

        for(i = 0; i < msj-2; i++){
            envio = i;
            MPI_Send(&envio, 1, MPI_INT, (i%2)+1, (rand()%100)+1, MPI_COMM_WORLD);
        }
            /*le enviamos un msj al proceso con RANK indicado por el i
            (utilizamos una etiqueta aleatoria). Cada proceso la recibe y muestra un mensaje si le toco una etiqueta
            correspondiente a el (el RANK 1 muestra pares, RANK 2 impares)*/

        MPI_Send(&envio, 1, MPI_INT, 1, TAG_EXIT, MPI_COMM_WORLD);
        MPI_Send(&envio, 1, MPI_INT, 2, TAG_EXIT, MPI_COMM_WORLD);
        //Enviamos estos dos mensajes para que los procesos de RANK 1 y 2 terminen
        
    } else if (rank == 1){
        
        //los demas procesos reciben
        
        while(1){
            
            MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            //Vemos que nos mandaron
            
            if(status.MPI_TAG == TAG_EXIT){
                printf("RANK %d: Recibi TAG %d. Me fui\n\n", rank, status.MPI_TAG);
                break;
            }
            
            
            
            if(esPar(status.MPI_TAG)){
                MPI_Recv(&mensaje, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
                printf("RANK %d (recibo tags pares), recibí un %d (SOURCE %d, TAG %d)\n", 
                       rank, mensaje, status.MPI_SOURCE, status.MPI_TAG);
            }
            else{
                //aca se tendria que descartar el mensaje y poder recibir otro
                
                MPI_Recv(&mensaje, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            }
        }

    }else{

        while(1){

            MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            //Vemos que nos mandaron

            if(status.MPI_TAG == TAG_EXIT){
                printf("RANK %d: Recibi TAG %d. Me fui\n\n", rank, status.MPI_TAG);
                break;
            }
            
            
            if(!(esPar(status.MPI_TAG))){
                MPI_Recv(&mensaje, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
                printf("RANK %d (recibo tags impares), recibí un %d (SOURCE %d, TAG %d)\n", 
                       rank, mensaje, status.MPI_SOURCE, status.MPI_TAG);
            }
            else{

                //aca se tendria que descartar el mensaje y poder recibir otro
                
                MPI_Recv(&mensaje, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            }
        }
    }
    
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
    return 0;
}