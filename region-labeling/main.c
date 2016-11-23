#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//#include <mpi.h>

#define N 10

/*m1 is the cloneable matrix, m2 is the resulting matrix*/
void clone_matrix(int m1[N][N], int m2[N][N]){
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            m2[i][j] = m1[i][j];
        }
    }
}
int isLit(int val){
    return val;
}

void print_matrix(int matrix[N][N]){
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
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

}

void random_initialize_matrix(int image[N][N]){
    srand(getpid());
    int posx = 0, 
        posy = 0;

    for(int i = 0; i < N*N; i++){
        
        posx = rand() % N;
        posy = rand() % N;

        image[posx][posy] = 1;

    }
}

void assign_label(int image[N][N], int label[N][N]){
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            if(isLit(image[i][j])){
                label[i][j] = i*10+j+1;
            }
        }

    }

}

int main(int argc, char const *argv[])
{   
    int image[N][N] = {0};
    int label[N][N] = {0};
    //every lit pixel will get an unique label

    initialize_matrix(image);
    //random_initialize_matrix(image);
    assign_label(image, label);

    print_matrix(image);
    printf("\n");
    print_matrix(label);


    return 0;
}