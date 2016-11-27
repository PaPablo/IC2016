#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define ROWS_PER_PROC       3
#define COORDINATOR_RANK    0
#define COORDINATOR_TAG     80

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

int isLit(int val){
    return val;
}

void print_matrix(int nrows, int ncols, int matrix[nrows][ncols]){
    for(int i = 0; i < nrows; i++){
        for(int j = 0; j < ncols; j++){
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void init_matrix(int size, int matrix[size][size], int val){
    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            matrix[i][j] = val;
        }
    }
}



void random_initialize_matrix(int size, int image[size][size]){
    srand(getpid());
    int posx = 0, 
        posy = 0;

    for(int i = 0; i < size*size; i++){
        
        posx = rand() % size;
        posy = rand() % size;

        image[posx][posy] = 1;

    }
}

void assign_label(int size, int image[size][size], int label[size][size]){
    int count = 1;
    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            if(isLit(image[i][j])){
                label[i][j] = count++;
            }
        }

    }

}


void coordinate(int *cont, int size, int rank){
    
    int change, acum;

    for(int i = 0; i < size; i++){
    //Coordinator receives messagges from processes (including itself)
    //and ends the execution if nobody made any changes
        MPI_Recv(&change, 1, MPI_INT, MPI_ANY_SOURCE, COORDINATOR_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        acum += change;
    }

    if(acum == 0){
        *cont = 0;
        MPI_Bcast(&cont, 1, MPI_INT, rank, MPI_COMM_WORLD);
    }
}

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

void update_upper_strip(int nrows, int ncols, int label[nrows][ncols], int lower_bound[ncols], int *change){
    int aux;
    for(int i = 0; i < nrows; i++){
        for(int j = 0; j < ncols; j++){
            aux = label[i][j];
            if(i == 0){
            //upper row (topest row of label matrix)
                if(j == 0){
                    //upper left corner
                    label[i][j] = max_corner(label[i+1][j], label[i][j+1]);
                }else if(j == ncols){
                    //upper right corner
                    label[i][j] = max_corner(label[i+1][j], label[i][j-1]);
                    
                }else{
                    //upper row, not corner
                    label[i][j] = max_edge(label[i][j-1], label[i+1][j], label[i][j+1]);
                }
            }else if(i == nrows){
            //lower row
                if(j == 0){
                    //lower left corner
                    label[i][j] = max_edge(label[i-1][j], label[i][j+1], lower_bound[j]);
                }else if(j == ncols){
                    //lower right corner
                    label[i][j] = max_edge(label[i-1][j], label[i][j-1], lower_bound[j]);
                }else{
                    //lower row, not corner
                    label[i][j] = max_inside(label[i][j-1], label[i+1][j], label[i][j+1], lower_bound[j]);
                }
            }else{
            //inside row
                if(j == 0){
                    //left edge
                    label[i][j] = max_edge(label[i-1][j], label[i][j+1], label[i-1][j]);
                }else if (j == ncols){
                    //right edge
                    label[i][j] = max_edge(label[i-1][j], label[i][j-1], label[i-1][j]);
                }else{
                    //inside node
                    label[i][j] = max_inside(label[i-1][j], label[i][j+1], label[i+1][j], label[i][j-1]);
                }
            }

            if(aux != label[i][j]){
                *change = 1;
            }
        }
    }
}

void update_lower_strip(int nrows, int ncols, int label[nrows][ncols], int upper_bound[ncols], int *change){
    int aux;
    for(int i = 0; i < nrows; i++){
        for(int j = 0; j < ncols; j++){
            aux = label[i][j];
            if(i == 0){
            //upper row
                if(j == 0){
                    //upper left corner
                    label[i][j] = max_edge(upper_bound[j], label[i][j+1], label[i+1][j]);
                }else if(j == ncols){
                    //upper right corner
                    label[i][j] = max_edge(upper_bound[j], label[i][j-1], label[i+1][j]);
                }else{
                    //upper row, not corner
                    label[i][j] = max_inside(upper_bound[j], label[i][j+1], label[i+1][j], label[i][j-1]);
                }
            }else if (i == nrows){
            //lower row (deepest row of label matrix)
                if(j == 0){
                    //lower left corner
                    label[i][j] = max_corner(label[i-1][j], label[i][j+1]);
                }else if (j == ncols){
                    //lower right corner
                    label[i][j] = max_corner(label[i-1][j], label[i][j-1]);
                }else{
                    //lower row, not corner
                    label[i][j] = max_edge(label[i][j-1], label[i-1][j], label[i][j+1]);
                }
            }else{
            //inside row
                if(j == 0){
                    //left edge
                    label[i][j] = max_edge(label[i-1][j], label[i][j+1], label[i+1][j]);
                }else if (j == ncols){
                    //right edge
                    label[i][j] = max_edge(label[i-1][j], label[i][j-1], label[i+1][j]);
                }else{
                    //inside node
                    label[i][j] = max_inside(label[i-1][j], label[i][j+1], label[i+1][j], label[i][j-1]);
                }
            }

            if(aux != label[i][j]){
                *change = 1;
            }
        }
    }
}

void update_inside_strip(int nrows, int ncols, int label[nrows][ncols], int upper_bound[ncols], int lower_bound[ncols], int *change){
    int aux;
    for(int i = 0; i < nrows; i++){
        for(int j = 0; j < ncols; j++){
            aux = label[i][j];
            if(i == 0){
            //upper row
                if(j == 0){
                    //upper left corner
                    label[i][j] = max_edge(upper_bound[j], label[i][j+1], label[i+1][j]);
                }else if(j == ncols){
                    //upper right corner
                    label[i][j] = max_edge(upper_bound[j], label[i][j-1], label[i+1][j]);
                }else{
                    //upper row, not corner
                    label[i][j] = max_inside(upper_bound[j], label[i][j+1], label[i+1][j], label[i][j-1]);
                }
            }else if(i == nrows){
            //lower row
                if(j == 0){
                    //lower left corner
                    label[i][j] = max_edge(label[i-1][j], label[i][j+1], lower_bound[j]);
                }else if (j == ncols){
                    //lower right corner
                    label[i][j] = max_edge(label[i-1][j], label[i][j-1], lower_bound[j]);
                }else{
                    //lower row, not corner
                    label[i][j] = max_inside(label[i-1][j], label[i][j+1], lower_bound[j], label[i][j-1]);
                }
            }else{
            //inside row
                if(j == 0){
                    //left edge
                    label[i][j] = max_edge(label[i-1][j], label[i][j+1], label[i+1][j]);
                }else if(j == ncols){
                    //right edge
                    label[i][j] = max_edge(label[i-1][j], label[i][j-1], label[i+1][j]);
                }else{
                    //inside node
                    label[i][j] = max_inside(label[i-1][j], label[i][j+1], label[i+1][j], label[i][j-1]);
                }
            }

            if(aux != label[i][j]){
                *change = 1;
            }
        }
    }
}

int main(int argc, char const *argv[]){   

    int world_size, rank;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int matrix_size = world_size * ROWS_PER_PROC;
    int image[matrix_size][matrix_size];
    int label[matrix_size][matrix_size];

    if(rank == 0){            
        init_matrix(matrix_size, image, 0);
        
        init_matrix(matrix_size, label, 0);
        //every lit pixel will get an unique label

        //initialize_matrix(matrix_size, image);
        random_initialize_matrix(matrix_size, image);
        assign_label(matrix_size, image, label);

        /*
        printf("LABEL MATRIX\n");
        print_matrix(matrix_size, matrix_size, label);
        printf("\n");*/
    }

    int local_label[ROWS_PER_PROC][matrix_size];   


    //we assign a strip of the label matrix to every process
    //each strip has ROW_PER_PROC rows 
    MPI_Scatter(label, (matrix_size*ROWS_PER_PROC), MPI_INT, 
                local_label, (matrix_size*ROWS_PER_PROC), MPI_INT, 
                0, MPI_COMM_WORLD);


    int upper_bound[matrix_size], 
        lower_bound[matrix_size], 
        cont = 1,
        change = 0;


    MPI_Barrier(MPI_COMM_WORLD);

    while(cont){
        if(rank != 0){
            MPI_Send(local_label[0], matrix_size, MPI_INT, rank-1, 0, MPI_COMM_WORLD);
            //send upper row of the strip to upper neighbor
        }
        if(rank != world_size-1){
            MPI_Send(local_label[ROWS_PER_PROC-1], matrix_size, MPI_INT, rank+1, 0, MPI_COMM_WORLD);
            //send lower row of the strip to lower neighbor
        }
        if(rank != world_size-1){
            MPI_Recv(upper_bound, matrix_size, MPI_INT, rank+1, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            //receive upper row from lower neighbor
        }
        if(rank != 0){
            MPI_Recv(lower_bound, matrix_size, MPI_INT, rank-1, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            //receive lower row from upper neighbor
        }

        //print_matrix()

        
        //update values
        if(rank == 0){
            update_upper_strip(ROWS_PER_PROC, matrix_size, local_label, lower_bound, &change);
        }
        else if(rank == (world_size - 1)){
            update_lower_strip(ROWS_PER_PROC, matrix_size, local_label, upper_bound, &change);
        }
        else{
            update_inside_strip(ROWS_PER_PROC, matrix_size, local_label, upper_bound, lower_bound, &change);
        }

        //send to coordinator if something changed
        MPI_Send(&change, 1, MPI_INT, COORDINATOR_RANK, COORDINATOR_TAG, MPI_COMM_WORLD);

        if(rank == COORDINATOR_RANK){
            coordinate(&cont, world_size, rank);
        }

        //receive answer from coordinator  
        MPI_Bcast(&cont, 1, MPI_INT, COORDINATOR_RANK, MPI_COMM_WORLD);

    }


    //now we could rebuild the original matrix
    //and print it
    MPI_Gather(local_label, (ROWS_PER_PROC*matrix_size), MPI_INT, 
        label, (ROWS_PER_PROC*matrix_size), MPI_INT, 
        COORDINATOR_RANK, MPI_COMM_WORLD);

    if(rank = COORDINATOR_RANK){
        print_matrix(matrix_size, matrix_size, label);
    }
  
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
    return 0;
}
