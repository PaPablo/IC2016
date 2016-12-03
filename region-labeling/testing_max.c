#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int max(int x, int y){
    if(x >= y)
        return x;

    return y;
}

int max_corner(int x, int y, int original){
    if(x > y){
        if(x > original)
            return x;
    }

    return max(y, original);
}

int max_edge(int x, int y, int z, int original){
    if(x > y){
        if(x > z){
            if(x > original)
                return x;
        }
    }

    return max_corner(y, z, original);
}


int max_inside(int x, int y, int z, int w, int original){
    if(x > y){
        if(x > z){
            if(x > w){
                if(x > original)
                    return x;
            }
        }
    }

    return max_edge(y, z, w, original);
} 

int digits(unsigned int num){
/*Cuenta cuantos digitos tiene un numero*/
    
    int contador=1;
    while(num/10>0)
    {
        num=num/10;
        contador++;
    }
    return contador;
}

void print_number(int num, int cant_num){ 
    int cant = digits(num);
    if(cant < cant_num){
        for(int i = 0; i < abs(cant_num - cant); i++){
            printf("%d", 0);
        }
    }    
    int val = num,
        p;
    for(int i = cant; i > 0; i--){
        p = (int)pow(10, i-1);
        printf("%d", val/p);
        val = val % p;
    }
    printf(" ");
}


int main(int argc, char const *argv[])
{

    
    print_number(180, 10);
    printf("\n");
    /*
    int x = 1,
        y = 2,
        z = 3,
        w = 4,
        t = 5;

    printf("MAX between %d, %d, %d, %d, %d =  %d\n", x, y, z, w, t, max_inside(x,y,z,w,t));
    printf("MAX between %d, %d, %d, %d, %d =  %d\n", t, w, z, y, x, max_inside(x,y,z,w,t));
    printf("MAX between %d, %d, %d, %d, %d =  %d\n", y, x, z, w, t, max_inside(x,y,z,w,t));
    printf("MAX between %d, %d, %d, %d, %d =  %d\n", y, z, x, w, t, max_inside(x,y,z,w,t));
    x = 5;
    y = 5;
    t = 10;
    printf("MAX between %d, %d, %d, %d, %d =  %d\n", x, y, z, w, t, max_inside(x,y,z,w,t));
    */
    return 0;
}