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

void print_row(int ncols, int row[ncols]){
    for(int i = 0; i < ncols; i++){
        printf("%d ", row[i]);
    }
}

void init_matrix(int size, int matrix[size][size], int val){
    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            matrix[i][j] = val;
        }
    }
}

void init_array(int size, int array[size], int val){
    for(int i = 0; i < size; i++){
        array[i] = val;
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


int coordinate(int chg, int size, int rank){
    
    int change, 
        acum = 0;

    MPI_Status status;
    //Coordinator receives messagges from every process 
    //and broadcast to finish the execution if nobody made any changes
    for(int i = 0; i < size-1; i++){
        MPI_Recv(&change, 1, MPI_INT, MPI_ANY_SOURCE, COORDINATOR_TAG, MPI_COMM_WORLD, &status);
        //printf("received a %d from process %d\n", change, status.MPI_SOURCE);
        acum += change;
    }
    acum += chg;
    //printf("COORDINATE WILL RETURN %d (change = %d. chg = %d)\n", acum, change, chg);
    return acum;

    /*
    *cont = acum;
    MPI_Bcast(&acum, 1, MPI_INT, rank, MPI_COMM_WORLD);*/
    
}

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

void update_upper_strip(int nrows, int ncols, int label[nrows][ncols], int lower_bound[ncols], int *change){
    //print_row(ncols, lower_bound);
    int aux;
    for(int i = 0; i < nrows; i++){
        for(int j = 0; j < ncols; j++){

            //if the pixel is not lit, we skip it
            if(!(isLit(label[i][j])))
                continue;

            aux = label[i][j];
            if(i == 0){
            //upper row (topest row of label matrix)
                if(j == 0){
                    //upper left corner
                    label[i][j] = max_corner(label[i+1][j], label[i][j+1], label[i][j]);
                }else if(j == (ncols-1)){
                    //upper right corner
                    label[i][j] = max_corner(label[i+1][j], label[i][j-1], label[i][j]);
                }else{
                    //upper row, not corner
                    label[i][j] = max_edge(label[i][j-1], label[i+1][j], label[i][j+1], label[i][j]);
                }
            }else if(i == (nrows-1)){
            //lower row
                if(j == 0){
                    //lower left corner
                    //printf("\nlower_bound[%d] = %d\n", j, lower_bound[j]);
                    //printf("%d %d = %d %d %d %d\n", i, j, label[i-1][j], label[i][j+1], lower_bound[j], label[i][j]);
                    label[i][j] = max_edge(label[i-1][j], label[i][j+1], lower_bound[j], label[i][j]);
                    //printf("label[%d][%d] = %d\n", i, j, label[i][j]);
                }else if(j == (ncols-1)){
                    //lower right corner
                    //printf("\nlower_bound[%d] = %d\n", j, lower_bound[j]);
                    //printf("%d %d = %d %d %d %d\n", i, j, label[i-1][j], label[i][j-1], lower_bound[j], label[i][j]);
                    label[i][j] = max_edge(label[i-1][j], label[i][j-1], lower_bound[j], label[i][j]);
                    //printf("label[%d][%d] = %d\n", i, j, label[i][j]);
                }else{
                    //lower row, not corner
                    //printf("\nlower_bound[%d] = %d\n", j, lower_bound[j]);
                    //printf("%d %d = %d %d %d %d %d\n", i, j, label[i][j-1], label[i+1][j], label[i][j+1], lower_bound[j], label[i][j]);
                    label[i][j] = max_inside(label[i][j-1], label[i-1][j], label[i][j+1], lower_bound[j], label[i][j]);
                    //printf("label[%d][%d] = %d\n", i, j, label[i][j]);  
                }
            }else{
            //inside row
                if(j == 0){
                    //left edge
                    label[i][j] = max_edge(label[i-1][j], label[i][j+1], label[i+1][j], label[i][j]);
                }else if (j == (ncols-1)){
                    //right edge
                    label[i][j] = max_edge(label[i-1][j], label[i][j-1], label[i+1][j], label[i][j]);
                }else{
                    //inside node
                    label[i][j] = max_inside(label[i-1][j], label[i][j+1], label[i+1][j], label[i][j-1], label[i][j]);
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

            //if the pixel is not lit, we skip it
            if(!(isLit(label[i][j])))
                continue;

            aux = label[i][j];
            if(i == 0){
            //upper row
                if(j == 0){
                    //upper left corner
                    label[i][j] = max_edge(upper_bound[j], label[i][j+1], label[i+1][j], label[i][j]);
                }else if(j == (ncols-1)){
                    //upper right corner
                    label[i][j] = max_edge(upper_bound[j], label[i][j-1], label[i+1][j], label[i][j]);
                }else{
                    //upper row, not corner
                    label[i][j] = max_inside(upper_bound[j], label[i][j+1], label[i+1][j], label[i][j-1], label[i][j]);
                }
            }else if (i == (nrows-1)){
            //lower row (deepest row of label matrix)
                if(j == 0){
                    //lower left corner
                    label[i][j] = max_corner(label[i-1][j], label[i][j+1], label[i][j]);
                }else if (j == (ncols-1)){
                    //lower right corner
                    label[i][j] = max_corner(label[i-1][j], label[i][j-1], label[i][j]);
                }else{
                    //lower row, not corner
                    label[i][j] = max_edge(label[i][j-1], label[i-1][j], label[i][j+1], label[i][j]);
                }
            }else{
            //inside row
                if(j == 0){
                    //left edge
                    label[i][j] = max_edge(label[i-1][j], label[i][j+1], label[i+1][j], label[i][j]);
                }else if (j == (ncols-1)){
                    //right edge
                    label[i][j] = max_edge(label[i-1][j], label[i][j-1], label[i+1][j], label[i][j]);
                }else{
                    //inside node
                    label[i][j] = max_inside(label[i-1][j], label[i][j+1], label[i+1][j], label[i][j-1], label[i][j]);
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

            //if the pixel is not lit, we skip it
            if(!(isLit(label[i][j])))
                continue;

            aux = label[i][j];
            if(i == 0){
            //upper row
                if(j == 0){
                    //upper left corner
                    label[i][j] = max_edge(upper_bound[j], label[i][j+1], label[i+1][j], label[i][j]);
                }else if(j == (ncols-1)){
                    //upper right corner
                    label[i][j] = max_edge(upper_bound[j], label[i][j-1], label[i+1][j], label[i][j]);
                }else{
                    //upper row, not corner
                    label[i][j] = max_inside(upper_bound[j], label[i][j+1], label[i+1][j], label[i][j-1], label[i][j]);
                }
            }else if(i == (nrows-1)){
            //lower row
                if(j == 0){
                    //lower left corner
                    label[i][j] = max_edge(label[i-1][j], label[i][j+1], lower_bound[j], label[i][j]);
                }else if (j == (ncols-1)){
                    //lower right corner
                    label[i][j] = max_edge(label[i-1][j], label[i][j-1], lower_bound[j], label[i][j]);
                }else{
                    //lower row, not corner
                    label[i][j] = max_inside(label[i-1][j], label[i][j+1], lower_bound[j], label[i][j-1], label[i][j]);
                }
            }else{
            //inside row
                if(j == 0){
                    //left edge
                    label[i][j] = max_edge(label[i-1][j], label[i][j+1], label[i+1][j], label[i][j]);
                }else if(j == (ncols-1)){
                    //right edge
                    label[i][j] = max_edge(label[i-1][j], label[i][j-1], label[i+1][j], label[i][j]);
                }else{
                    //inside node
                    label[i][j] = max_inside(label[i-1][j], label[i][j+1], label[i+1][j], label[i][j-1], label[i][j]);
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

    int strip[ROWS_PER_PROC][matrix_size];   


    //we assign a strip of the label matrix to every process
    //each strip has ROW_PER_PROC rows 
    MPI_Scatter(label, (matrix_size*ROWS_PER_PROC), MPI_INT, 
                strip, (matrix_size*ROWS_PER_PROC), MPI_INT, 
                0, MPI_COMM_WORLD);


    int upper_bound[matrix_size], 
        lower_bound[matrix_size], 
        cont = 1,
        change;

    init_array(matrix_size, upper_bound, 0);
    init_array(matrix_size, lower_bound, 0);  

    MPI_Barrier(MPI_COMM_WORLD);

    /*
    if(rank != 0){
        MPI_Send(strip[0], matrix_size, MPI_INT, rank-1, 0, MPI_COMM_WORLD);
        //send upper row of the strip to upper neighbor
    }
    if(rank != (world_size-1)){
        MPI_Send(strip[ROWS_PER_PROC-1], matrix_size, MPI_INT, rank+1, 0, MPI_COMM_WORLD);
        //send lower row of the strip to lower neighbor
    }
    if(rank != (world_size-1)){
        MPI_Recv(lower_bound, matrix_size, MPI_INT, rank+1, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        //receive upper row from lower neighbor
    }
    if(rank != 0){
        MPI_Recv(upper_bound, matrix_size, MPI_INT, rank-1, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        //receive lower row from upper neighbor
    }


    if(rank == 0){
        printf("\nPROCESS %d STRIP:\n\n", rank);
        print_matrix(ROWS_PER_PROC, matrix_size, strip);
        printf("\nPROCESS %d LOWER BOUND:\n", rank);
        print_row(matrix_size, lower_bound);
    }else if (rank == (world_size - 1)){
        printf("\nPROCESS %d UPPER BOUND:\n", rank);
        print_row(matrix_size, upper_bound);
        printf("\nPROCESS %d STRIP:\n\n", rank);
        print_matrix(ROWS_PER_PROC, matrix_size, strip);   
    }else{
        printf("\n\nPROCESS %d UPPER BOUND:\n", rank);
        print_row(matrix_size, upper_bound);
        printf("\nPROCESS %d STRIP:\n", rank);
        print_matrix(ROWS_PER_PROC, matrix_size, strip);
        printf("\nPROCESS %d LOWER BOUND:\n\n", rank);
        print_row(matrix_size, lower_bound);  
    }*/

    /*
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
    exit(0);*/

    while(cont){
        change = 0;

        if(rank != 0){
            MPI_Send(strip[0], matrix_size, MPI_INT, rank-1, 0, MPI_COMM_WORLD);
            //send upper row of the strip to upper neighbor
        }
        if(rank != (world_size-1)){
            MPI_Send(strip[ROWS_PER_PROC-1], matrix_size, MPI_INT, rank+1, 0, MPI_COMM_WORLD);
            //send lower row of the strip to lower neighbor
        }
        if(rank != (world_size-1)){
            MPI_Recv(lower_bound, matrix_size, MPI_INT, rank+1, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            //receive upper row from lower neighbor
        }
        if(rank != 0){
            MPI_Recv(upper_bound, matrix_size, MPI_INT, rank-1, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            //receive lower row from upper neighbor
        }
        

        //update values
        if(rank == 0){
            printf("im gonna update upper strip\n");
            update_upper_strip(ROWS_PER_PROC, matrix_size, strip, lower_bound, &change);
        }
        else if(rank == (world_size - 1)){
            printf("im gonna update lower strip\n");
            update_lower_strip(ROWS_PER_PROC, matrix_size, strip, upper_bound, &change);
        }
        else{
            printf("im gonna update inside strip\n");
            update_inside_strip(ROWS_PER_PROC, matrix_size, strip, upper_bound, lower_bound, &change);
        }

        if(rank != COORDINATOR_RANK){
            //send to coordinator if something changed
            MPI_Send(&change, 1, MPI_INT, COORDINATOR_RANK, COORDINATOR_TAG, MPI_COMM_WORLD);
        }

        if(rank == COORDINATOR_RANK){
            cont = coordinate(change, world_size, rank);

            //MPI_Bcast(&cont, 1, MPI_INT, rank, MPI_COMM_WORLD);
        }


        //Broadcast continue flag 
        MPI_Bcast(&cont, 1, MPI_INT, COORDINATOR_RANK, MPI_COMM_WORLD); 

    }

    //printf("PROCESS %d: JUST GOT OUT OF THE WHILE LOOP YEYYYYYYYYYYYYYYYYYYYYYYaaaaaaaaaaaaaaa xD\n", rank);
    MPI_Barrier(MPI_COMM_WORLD);
    
    //printf("PROCESS %d: JUST GOT OUT OF THE WHILE LOOP YEYYYYYYYYYYYYYYYYYYYYYYaaaaaaaaaaaaaaa xD\n", rank);

    
    //now we could rebuild the original matrix
    //and print it

    int new_label[matrix_size][matrix_size];

    MPI_Gather(strip, (ROWS_PER_PROC*matrix_size), MPI_INT, 
    new_label, (ROWS_PER_PROC*matrix_size), MPI_INT, 
    COORDINATOR_RANK, MPI_COMM_WORLD);

    if(rank == COORDINATOR_RANK){
        printf("\n\nORIGINAL LABEL MATRIX\n");
        print_matrix(matrix_size, matrix_size, label);
        printf("\n\n");
        printf("BRAND NEW LABEL MATRIX\n");
        print_matrix(matrix_size, matrix_size, new_label);
    }
  
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
    return 0;
}
