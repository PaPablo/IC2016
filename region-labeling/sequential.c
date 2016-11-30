#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#define NUMBERS_PER_CELL    3

/*m1 is the cloneable matrix, m2 is the resulting matrix*/

void clone_matrix(int size, int m1[size][size], int m2[size][size]){
    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            m2[i][j] = m1[i][j];
        }
    }
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

void update_values(int nrows, int ncols, int label[nrows][ncols], int *change){

    *change = 1;
    
    for(int i = 0; i < nrows; i++){
        for(int j = 0; j < ncols; j++){
            if(!isLit(label[i][j]))
                continue;

            int aux = label[i][j];

            if(i == 0){
            //topest row of label matrix
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
            }else if (i == (nrows-1)){
            //deepest row of label matrix
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

            if(aux != label[i][j])
                *change = 0;
        }
    }
}
int main(int argc, char const *argv[])
{
    if ((argc < 2) || (atoi(argv[1]) < 2)) {
        printf("USAGE: ./sequential {matrix matrix_size (has to be greater than 2)}\n");
        exit(1);
    }

    int matrix_size = atoi(argv[1]);

    int image[matrix_size][matrix_size];
    int label[matrix_size][matrix_size];

    init_matrix(matrix_size, image, 0);
    init_matrix(matrix_size, label, 0);

    random_initialize_matrix(matrix_size, image);
    assign_label(matrix_size, image, label);
    
    int change = 0;
    
    int new_label[matrix_size][matrix_size];

    clone_matrix(matrix_size, label, new_label);


    

    while(!change){
        update_values(matrix_size, matrix_size, label, &change);
    }

    printf("SEQUENTIAL VERSION\n");
    printf("IMAGE MATRIX\n");
    print_matrix(matrix_size, matrix_size, image);
    printf("\n\nORIGINAL LABEL MATRIX\n");
    print_matrix(matrix_size, matrix_size, new_label);
    printf("\n\n");
    printf("BRAND NEW LABEL MATRIX\n");
    print_matrix(matrix_size, matrix_size, label);

    return 0;
}