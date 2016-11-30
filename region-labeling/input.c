/*m1 is the cloneable matrix, m2 is the resulting matrix*/
/*
void clone_matrix(int size, int m1[size][size], int m2[size][size]){
    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            m2[i][j] = m1[i][j];
        }
    }
}


void initialize_matrix(int image[N][N]){

    int mtx[N][N] =
    {
        {1, 0, 1, 0, 0, 0, 0, 0, 1, 1},
        {0, 1, 1, 1, 1, 0, 0, 0, 1, 1},
        {1, 1, 0, 1, 1, 1, 0, 1, 1, 1},
        {1, 1, 1, 0, 1, 1, 0, 0, 0, 1},
        {1, 1, 0, 1, 1, 1, 1, 0, 1, 0},
        {0, 0, 0, 1, 0, 1, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 0, 0, 0, 0, 0, 0, 1, 1},
        {1, 1, 1, 0, 0, 0, 0, 1, 1, 1},
        {1, 1, 1, 1, 0, 0, 1, 1, 1, 1},
    };

    clone_matrix(mtx, image);

}*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char const *argv[])
{

    //int arg = atoi(argv[1]);
    //char cadena[] = "Hola!";

    if(argc < 2){
        printf("USAGE BLABLABLA\n");
        exit(1);
    }
    /*
    int size = atoi(argv[1]);
    if(size > 1){
        printf("DALE GUACHO TODO PIOLA INGRESASTE UN %d\n", size);
    }

    printf("%d\n", argc);
    /*
    for(int i = 0; i < (int)strlen(*argv); i++){
        printf("%c\n", (char)argv[i]);
    }*/

    printf("%s\n", argv[1]);
    //printf("%d\n", (int)strlen(*argv));
    return 0;
}