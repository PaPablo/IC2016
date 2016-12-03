#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

#define COORDINATOR_RANK    0
#define COORDINATOR_TAG     80

#define NUMBERS_PER_CELL    3

int sum(int max){
    int acum = 0;
    for(int i = 0; i <= max; i++){
        acum += i;
    }
    return acum;
}

int sum_array(int max, int array[max]){
    int acum = 0;
    for(int i = 0; i < max; i++){
        acum += array[i];
    }

    return acum;
}

int digits(unsigned int num){
/*Counts how many digits has a number*/
    
    int count = 1;
    while(num/10>0)
    {
        num=num/10;
        count++;
    }
    return count;
}

void print_number(int num, int cant_num){ 
    int cant = digits(num);
    if(cant < cant_num){
        for(int i = 0; i < abs(cant_num - cant); i++){
            printf(" ");
            //printf("0");
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

int isLit(int val){
    return val;
}

void print_matrix(int nrows, int ncols, int matrix[nrows][ncols]){
    for(int i = 0; i < nrows; i++){
        for(int j = 0; j < ncols; j++){
            print_number(matrix[i][j], NUMBERS_PER_CELL);
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


int coordinate(int *flags, int rank, int world_size){
    for(int i = 0; i < world_size; i++){
        //we do not send messages to ourself
        if(i == rank) continue;

        MPI_Send(&flags[rank], 1, MPI_INT, i, 0, MPI_COMM_WORLD);

        MPI_Recv(&flags[i], 1, MPI_INT, i, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    /*returns a 0 if the summation between 1 and world_size equals to
    the summation of the elements in the array flags*/
    return !(sum(world_size) == sum_array(world_size, flags));
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
                    label[i][j] = max_edge(label[i-1][j], label[i][j+1], lower_bound[j], label[i][j]);
                }else if(j == (ncols-1)){
                    //lower right corner
                    label[i][j] = max_edge(label[i-1][j], label[i][j-1], lower_bound[j], label[i][j]);
                }else{
                    //lower row, not corner
                    label[i][j] = max_inside(label[i][j-1], label[i-1][j], label[i][j+1], lower_bound[j], label[i][j]); 
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

    if ((argc < 2) || (atoi(argv[1]) < 2)) {
        printf("USAGE: ./no_coordinator {rows per proc (has to be greater than 2)}\n");
        exit(1);
    }

    int rows_per_proc = atoi(argv[1]);

    int world_size, rank;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int matrix_size = world_size * rows_per_proc;
    int image[matrix_size][matrix_size];
    int label[matrix_size][matrix_size];

    if(rank == 0){            
        init_matrix(matrix_size, image, 0);
        
        init_matrix(matrix_size, label, 0);
        //every lit pixel will get an unique label

        //initialize_matrix(matrix_size, image);
        random_initialize_matrix(matrix_size, image);
        assign_label(matrix_size, image, label);

    }

    int strip[rows_per_proc][matrix_size];   


    //we assign a strip of the label matrix to every process
    //each strip has ROW_PER_PROC rows 
    MPI_Scatter(label, (matrix_size*rows_per_proc), MPI_INT, 
                strip, (matrix_size*rows_per_proc), MPI_INT, 
                0, MPI_COMM_WORLD);


    int upper_bound[matrix_size], 
        lower_bound[matrix_size], 
        cont = 1,
        change,
        rnk = rank;

    if(rank == 0)
        rnk = world_size;
    //rnk will be between 1 and world_size
    int flags[world_size];

    init_array(world_size, flags, 0);
    init_array(matrix_size, upper_bound, 0);
    init_array(matrix_size, lower_bound, 0);  

    MPI_Barrier(MPI_COMM_WORLD);


    while(cont){
        change = 0;
        flags[rank] = 0;

        if(rank != 0){
            //send upper row of the strip to upper neighbor
            MPI_Send(strip[0], matrix_size, MPI_INT, rank-1, 0, MPI_COMM_WORLD);
        }
        if(rank != (world_size-1)){
            //send lower row of the strip to lower neighbor
            MPI_Send(strip[rows_per_proc-1], matrix_size, MPI_INT, rank+1, 0, MPI_COMM_WORLD);
        }
        if(rank != (world_size-1)){
            //receive upper row from lower neighbor
            MPI_Recv(lower_bound, matrix_size, MPI_INT, rank+1, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        if(rank != 0){
            //receive lower row from upper neighbor
            MPI_Recv(upper_bound, matrix_size, MPI_INT, rank-1, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        

        //update values
        if(rank == 0){
            update_upper_strip(rows_per_proc, matrix_size, strip, lower_bound, &change);
        }
        else if(rank == (world_size - 1)){
            update_lower_strip(rows_per_proc, matrix_size, strip, upper_bound, &change);
        }
        else{
            update_inside_strip(rows_per_proc, matrix_size, strip, upper_bound, lower_bound, &change);
        }


        if(!change)
            flags[rank] = rnk;
        
            

        cont = coordinate(flags, rank, world_size);

    }

    MPI_Barrier(MPI_COMM_WORLD);
    
    
    //now we rebuild the original matrix
    //and print it

    int new_label[matrix_size][matrix_size];

    MPI_Gather(strip, (rows_per_proc*matrix_size), MPI_INT, 
    new_label, (rows_per_proc*matrix_size), MPI_INT, 
    COORDINATOR_RANK, MPI_COMM_WORLD);
    

    if(rank == COORDINATOR_RANK){
        printf("This version DOES NOT implement a coordinator\n");
        printf("IMAGE MATRIX\n");
        print_matrix(matrix_size, matrix_size, image);
        printf("\n\nORIGINAL LABEL MATRIX\n");
        print_matrix(matrix_size, matrix_size, label);
        printf("\n\n");
        /*
        char c;
        scanf("%c", &c);*/
        
        printf("BRAND NEW LABEL MATRIX\n");
        print_matrix(matrix_size, matrix_size, new_label);
    }
  
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
    return 0;
}
