#include <stdio.h>
#define n 10

void ini_mtx(int matrix[n][n]){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            matrix[i][j] = i+j;
        }
    }
}


void print_matrix(int matrix[n][n]){

    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }

}

void print_row(int row[n]){
    for(int i = 0; i < n; i++){
        printf("%d ", row[i]);
    }
    printf("\n");
}



int main(int argc, char const *argv[])
{
    int matrix[n][n];

    ini_mtx(matrix);

    print_matrix(matrix);
    printf("\n\nnow we print a row\n");

    print_row(matrix[0]);

    printf("\n\n");
    



    return 0;
}