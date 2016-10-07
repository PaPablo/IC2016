
#include<stdio.h>
#include<unistd.h>
#include<omp.h>

int num_steps = 100000;
double step;
int main(){
    int i;
    double x, pi, sum;
    double incremento = (double)num_steps / (num_steps * 2); 
    
    step = 1.0/(double)num_steps;

    #pragma omp parallel for reduction(+:sum)
    for(i=0; i < num_steps; i++){
        x = (i + incremento)*step;
        sum += 4.0/(1.0+(x*x));
        usleep(1);
    }
    
    pi = step*sum;
    
    printf("PI = %.50f\n", pi);
    return 0;
}