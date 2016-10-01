
#include <omp.h>
#include <stdio.h>

int main(void){
    int ID = omp_get_thread_num();
    #pragma omp parallel num_threads(4)
    {
        printf("Hilo:%d\n", ID);
    }
}