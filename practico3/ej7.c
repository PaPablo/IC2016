
#include<stdio.h>
#include<unistd.h>

int num_steps = 100000;
double step;
int main(){
    int i;
    double x, pi, sum = 0.0;
    double incremento = (double)num_steps / (num_steps * 2); 
    
    printf("incremento = %.2f\n", incremento);
    
    step = 1.0/(double)num_steps;
    
    printf("step = %.5f\n", step);
    //sleep(1);
    
    for(i=0; i < num_steps; i++){
        x = (i + incremento)*step;
        sum += 4.0/(1.0+(x*x));
    }
    
    pi = step*sum;
    
    printf("PI = %.50f\n", pi);
    return 0;
}