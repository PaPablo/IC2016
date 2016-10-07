
#include <omp.h>
#include <stdio.h>

int main(void){
    
    #pragma omp parallel 
    {
        int ID = omp_get_thread_num();
        printf("Hilo:%d\n", ID);
    }
}