#include <stdio.h>

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


int main(int argc, char const *argv[])
{
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
    return 0;
}