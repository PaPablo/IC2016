#include <stdio.h>

#define N 4

int max_corner(int x, int y){
    if(x > y){
        return x;
    }
    return y;
}

int max_edge(int x, int y, int z){
    if(x > y){
        if(x > z){
            return x;
        }
    }

    return max_corner(y, z);
}


int max_inside(int x, int y, int z, int w){
    if(x > y){
        if(x > z){
            if(x > w){
                return x;
            }
        }
    }

    return max_edge(y, z, w);
}

void print_matrix(int x, int matrix[x][x]){

    for(int i = 0; i < x; i++){
        for(int j = 0; j < x; j++){
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }

}

void stuff(int *x){
    *x = 3;
}

void do_something(int n, int matrix[n][n]){
    printf("Element [0][0] = %d\n", matrix[0][0]);

    stuff(&matrix[0][0]);

    printf("Element [0][0] = %d\n", matrix[0][0]);
}

int main(int argc, char const *argv[])
{
    int m[N][N] = {
        {1, 2, 3},
        {9, 8, 7},
        {4, 5, 6},
    };

    char c;

    printf("PRESS A KEY TO CONTINUE...\n");
    scanf("%c", &c);
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            if(m[i][j] == 3) continue;

            printf("%d\n", m[i][j]);
        }
    }
    /*
    print_matrix(N, m);     

    
    printf("the greatest value between %d and %d is = %d\n\n", m[0][2], m[2][0],  max_corner(m[0][2], m[2][0]));
    printf("the greatest value between %d, %d, and %d is = %d\n\n", m[0][2], m[2][0], m[1][1],  max_edge(m[0][2], m[2][0], m[1][1]));
    printf("the greatest value between %d, %d, and %d is = %d\n\n", m[2][0], m[0][2], m[1][1],  max_edge(m[2][0], m[0][2], m[1][1]));
    printf("the greatest value between %d, %d, and %d is = %d\n\n", m[1][1], m[2][0], m[0][2],  max_edge(m[1][1], m[2][0], m[0][2]));
    printf("the greatest value between %d, %d, and %d is = %d\n\n", m[2][0], m[1][1], m[0][2],  max_edge(m[0][2], m[2][0], m[1][1]));

    printf("Now we test the max_inside...\n\n\n");

    printf("the greatest value between %d, %d, %d, and %d is = %d\n\n", m[0][2], m[2][0], m[1][1], m[1][0], max_inside(m[0][2], m[2][0], m[1][1], m[1][0]));
    /*
    printf("the greatest value between %d, %d, and %d is = %d\n\n", m[2][0], m[0][2], m[1][1],  max_edge(m[2][0], m[0][2], m[1][1]));
    printf("the greatest value between %d, %d, and %d is = %d\n\n", m[1][1], m[2][0], m[0][2],  max_edge(m[1][1], m[2][0], m[0][2]));
    printf("the greatest value between %d, %d, and %d is = %d\n\n", m[2][0], m[1][1], m[0][2],  max_edge(m[0][2], m[2][0], m[1][1]));*/

    //do_something(N, m);
    //print_matrix(N, m);    */
    return 0;
}






/*

        IMPRIMIR LOS STRIPS DE CADA PROCESO
        CON LA FILA DEL VECINO/S QUE RECIBE CADA UNO

        */
        /*
        if(rank == 0){
            printf("PROCESS %d STRIP\n", rank);
            print_matrix(ROWS_PER_PROC, matrix_size, local_label);
            printf("PROCESS %d LOWER BOUND\n", rank);
            print_row(matrix_size, lower_bound);
        }

        if(rank == 1){
            printf("\n\nPROCESS %d UPPER BOUND\n", rank);
            print_row(matrix_size, upper_bound);
            printf("\n");
            printf("PROCESS %d STRIP\n", rank);
            print_matrix(ROWS_PER_PROC, matrix_size, local_label);
            printf("\n");
            printf("PROCESS %d LOWER BOUND\n", rank);
            print_row(matrix_size, lower_bound);
        }

        if(rank == world_size-1){
            printf("\n\nPROCESS %d UPPER BOUND\n", rank);
            print_row(matrix_size, upper_bound);
            printf("\n");
            printf("PROCESS %d STRIP\n", rank);
            print_matrix(ROWS_PER_PROC, matrix_size, local_label);
            printf("\n");
        }

        finalize();*/